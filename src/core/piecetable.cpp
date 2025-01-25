#include "piecetable.h"

using namespace std;

PieceTable::PieceTable()
{
    PieceTable("");
}

PieceTable::PieceTable(const string &initialContent)
{
    originalBuffer = initialContent;
    const unsigned int bufferSize = originalBuffer.size();
    vector<unsigned int> lineStarts = {};
    for (unsigned int index = 0; index < bufferSize; index++)
    {
        if (originalBuffer[index] == '\n')
        {
            lineStarts.push_back(index + 1);
        }
    }
    pieces.push_back({sourceType::original, 0, static_cast<unsigned int>(initialContent.size()), lineStarts});
    documentLength = bufferSize;
    currentPiece = nullptr;
}

void PieceTable::Insert(unsigned int index, const string &text)
{
    if (index > documentLength)
    {
        throw out_of_range("Insert out of range");
    }

    unsigned int tsize = text.size();
    addBuffer += text;

    vector<unsigned int> newLineStarts = getLineStarts(text, documentLength, tsize);

    // No originalBuffer case
    if (pieces.empty())
    {
        Piece newPiece = {sourceType::add, 0, tsize, newLineStarts};
        pieces.push_back(newPiece);
        documentLength += tsize;
        currentPiece = &pieces.back();
        return;
    }

    // Appending but no current pointer is set yet case
    if (index == documentLength)
    {
        currentPiece = &pieces.back();
    }

    // Check if current piece can be directly used
    if (currentPiece != nullptr &&
        index >= currentPiece->start &&
        index <= (currentPiece->start + currentPiece->length))
    {
        // Extend current piece or add new piece based on lastDeletedIndex
        if (lastDeletedIndex < currentPiece->start + currentPiece->length &&
            lastDeletedIndex >= currentPiece->start)
        {
            // Create new piece
            pieces.push_back({sourceType::add,
                              currentPiece->start + currentPiece->length,
                              tsize,
                              newLineStarts});
            currentPiece = &pieces.back();
        }
        else
        {
            // Extend current piece
            currentPiece->length += tsize;
            currentPiece->lineStart.insert(
                currentPiece->lineStart.end(),
                newLineStarts.begin(),
                newLineStarts.end());
        }

        if (index < documentLength)
        {
            for (Piece *piece = currentPiece + 1; piece < &pieces.back() + 1; ++piece)
            {
                piece->start += tsize;
            }
        }

        documentLength += tsize;
        return;
    }
    // Middle insertion
    unsigned int currentLength = 0;
    for (auto it = pieces.begin(); it != pieces.end(); ++it)
    {
        unsigned int pieceEnd = currentLength + it->length;

        if (index < pieceEnd)
        {
            unsigned int localIndex = index - currentLength;

            // Cover case where we are (not)inserting exactly at start of piece
            if (localIndex > 0)
            {
                Piece prevPiece = {it->source,
                                   it->start,
                                   localIndex,
                                   getLineStarts(it->source, it->start, localIndex)};
                it = pieces.erase(it);
                it = pieces.insert(it, prevPiece);
                ++it;
            }

            Piece newPiece = {sourceType::add,
                              documentLength,
                              tsize,
                              newLineStarts};

            Piece nextPiece = {it->source,
                               it->start + localIndex + tsize,
                               it->length - localIndex,
                               getLineStarts(it->source, it->start + localIndex + tsize, it->length - localIndex)};

            it = pieces.insert(it, newPiece);
            currentPiece = &(*it);
            ++it;
            it = pieces.insert(it, nextPiece);
            for (; it != pieces.end(); ++it)
            {
                if (it->source == sourceType::add)
                {
                    it->start += tsize;
                }
            }

            documentLength += tsize;
            break;
        }
        currentLength += it->length;
    }
}

void PieceTable::Delete(unsigned int index, unsigned int deletionLength)
{
    if (index + deletionLength > documentLength)
    {
        throw out_of_range("Delete out of range");
    }
    if (deletionLength == 0)
    {
        return;
    }

    // Update lastDeletedIndex to the highest index of deletion
    lastDeletedIndex = index + deletionLength - 1;

    // Try to use currentPiece for optimization
    if (currentPiece &&
        index >= currentPiece->start &&
        index < (currentPiece->start + currentPiece->length))
    {
        unsigned int localIndex = index - currentPiece->start;
        unsigned int remainingInPiece = currentPiece->length - (localIndex + deletionLength);

        // Handle deletion within current piece
        if (remainingInPiece > 0)
        {
            // Left part of piece remains
            Piece leftPiece = {
                currentPiece->source,
                currentPiece->start,
                localIndex,
                getLineStarts(currentPiece->source, currentPiece->start, localIndex)};

            // Right part of piece remains
            Piece rightPiece = {
                currentPiece->source,
                currentPiece->start + localIndex + deletionLength,
                remainingInPiece,
                getLineStarts(currentPiece->source,
                              currentPiece->start + localIndex + deletionLength,
                              remainingInPiece)};

            // Replace current piece
            auto it = pieces.begin() + (currentPiece - &pieces[0]);

            pieces.erase(it);

            if (localIndex > 0)
            {
                pieces.insert(it, leftPiece);
            }

            if (remainingInPiece > 0)
            {
                pieces.insert(it, rightPiece);
            }

            documentLength -= deletionLength;
            currentPiece = nullptr;
            return;
        }
        else
        {
            // Entire current piece is deleted
            auto it = pieces.begin() + (currentPiece - &pieces[0]);
            pieces.erase(it);
            currentPiece = nullptr;
        }
    }

    // Fallback to general deletion logic if currentPiece doesn't cover full deletion
    unsigned int currentLength = 0;
    unsigned int remainingDeletion = deletionLength;

    for (auto it = pieces.begin(); it != pieces.end();)
    {
        unsigned int pieceEnd = currentLength + it->length;

        if (index < pieceEnd)
        {
            unsigned int localIndex = index - currentLength;
            unsigned int deletionInPiece = min(remainingDeletion, it->length - localIndex);

            // Handle piece splitting
            if (localIndex > 0)
            {
                // Left part of piece
                Piece leftPiece = {
                    it->source,
                    it->start,
                    localIndex,
                    getLineStarts(it->source, it->start, localIndex)};
                pieces.insert(it, leftPiece);
            }

            unsigned int remainingInPiece = it->length - (localIndex + deletionInPiece);
            if (remainingInPiece > 0)
            {
                // Right part of piece
                Piece rightPiece = {
                    it->source,
                    it->start + localIndex + deletionInPiece,
                    remainingInPiece,
                    getLineStarts(it->source,
                                  it->start + localIndex + deletionInPiece,
                                  remainingInPiece)};
                it = pieces.erase(it);
                it = pieces.insert(it, rightPiece);
            }
            else
            {
                // Remove entire piece
                it = pieces.erase(it);
            }

            remainingDeletion -= deletionInPiece;

            if (remainingDeletion == 0)
            {
                break;
            }
        }

        currentLength += it->length;
        ++it;
    }

    documentLength -= deletionLength;
    currentPiece = nullptr;
}

string PieceTable::getContent() const
{
    string content;
    for (const auto &piece : pieces)
    {
        const string &buffer = piece.source == sourceType::original ? originalBuffer : addBuffer;
        content += buffer.substr(piece.start, piece.length);
    }
    return content;
}

char PieceTable::getCharAt(unsigned int index) const
{
    if (index >= documentLength)
    {
        return '\0';
    }
    unsigned int overallIndex = 0;
    for (const auto &piece : pieces)
    {
        if (index < (piece.length + overallIndex) && index >= overallIndex)
        {
            const string &buffer = piece.source == sourceType::original ? originalBuffer : addBuffer;
            return buffer[index - overallIndex];
        }
        overallIndex += piece.length;
    }
    return '\0';
}

vector<unsigned int> PieceTable::getLineStarts(sourceType src, unsigned int start, unsigned int len) const
{
    string text = src == sourceType::original
                      ? originalBuffer.substr(start, len)
                      : addBuffer.substr(start, len);
    return getLineStarts(text, start, len);
}

vector<unsigned int> PieceTable::getLineStarts(string text, unsigned int tstart, unsigned int tsize) const
{
    vector<unsigned int> lineStarts = {};
    for (unsigned int index = 0; index < tsize; index++)
    {
        if (text[index] == '\n')
        {
            lineStarts.push_back(tstart + index + 1);
        }
    }
    return lineStarts;
}