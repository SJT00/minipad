#include "piecetable.h"
#include <iostream>
#include <iomanip>

using namespace std;

bool DEBUG = false;

PieceTable::PieceTable() : PieceTable("") {}

PieceTable::PieceTable(const string &initialContent)
{
    const unsigned int bufferSize = initialContent.size();
    if (bufferSize > 0)
    {
        originalBuffer = initialContent;
        pieces.push_back({sourceType::original, 0, static_cast<unsigned int>(bufferSize)});
        activePiece = {.cLen = 0, .idx = 0};
    }
    documentLength = bufferSize;

    Visualize();
}

void PieceTable::Insert(unsigned int rawIndex, const string &text)
{
    unsigned int tsize = text.size();
    if (tsize == 0)
    {
        return;
    }
    unsigned int index = min(max(rawIndex, static_cast<unsigned int>(0)), documentLength);
    Piece *currentPiece = GetActivePiece(index);
    addBuffer += text;
    if (currentPiece == nullptr)
        out_of_range("No Current Piece Found");

    unsigned int normalizedIdx = index - activePiece.cLen;
    // Appending Case
    if (normalizedIdx == currentPiece->length || currentPiece->length == 0)
    {
        if (currentPiece->source == original || (lastDeletedIndex >= currentPiece->start && lastDeletedIndex < currentPiece->start + currentPiece->length))
        {
            // TODO: FIX/REMOVE THIS ORIGINAL BUFFER CASE
            out_of_range("Delete Case Triggered");
            pieces.push_back({sourceType::add, documentLength, tsize});
            activePiece = {.idx = activePiece.idx + 1, .cLen = activePiece.cLen + currentPiece->length};
        }
        else
        {
            currentPiece->length += tsize;
        }
        documentLength += tsize;
        Visualize();
        return;
    }
    // Start or Middle Case
    Piece preSplitPiece = *currentPiece;
    auto currIter = pieces.begin() + activePiece.idx;
    if (normalizedIdx > 0)
    {
        Piece prevPiece = {preSplitPiece.source,
                           preSplitPiece.start,
                           normalizedIdx};
        currIter = pieces.erase(currIter);
        currIter = pieces.insert(currIter, prevPiece);
        ++currIter;
    }
    Piece middlePiece = {
        sourceType::add,
        static_cast<unsigned int>(addBuffer.size()) - tsize,
        tsize,
    };
    currIter = pieces.insert(currIter, middlePiece);
    activePiece = {.cLen = activePiece.cLen + normalizedIdx, .idx = normalizedIdx == 0 ? activePiece.idx : activePiece.idx + 1};

    if (normalizedIdx > 0 && normalizedIdx < preSplitPiece.length)
    {
        unsigned int newStart = preSplitPiece.start + normalizedIdx;
        Piece nextPiece = {preSplitPiece.source,
                           newStart,
                           preSplitPiece.length - normalizedIdx};
        ++currIter;
        currIter = pieces.insert(currIter, nextPiece);
    }

    documentLength += tsize;
    Visualize();
}

void PieceTable::Delete(unsigned int index, unsigned int deletionLength)
{

    Visualize();
}

string PieceTable::GetContent()
{
    string content;
    lineStarts = {0};
    for (const auto &piece : pieces)
    {
        const string &buffer = piece.source == sourceType::original ? originalBuffer : addBuffer;
        content += buffer.substr(piece.start, piece.length);
    }
    for (size_t i = 0; i < content.size(); ++i)
    {
        if (content[i] == '\n' && i + 1 < content.size())
        {
            lineStarts.push_back(i + 1);
        }
    }

    return content;
}

bool PieceTable::ValidActivePiece(unsigned int docIndex)
{
    if (activePiece.idx < pieces.size())
    {
        Piece *currentPiece = &pieces[activePiece.idx];
        if (docIndex < activePiece.cLen)
        {
            return false;
        }
        unsigned int normalizedIdx = docIndex - activePiece.cLen;
        if (normalizedIdx >= 0 && normalizedIdx <= currentPiece->length)
        {
            return true;
        }
    }
    return false;
}

Piece *PieceTable::GetActivePiece(unsigned int docIndex)
{
    return GetPiece(docIndex, true);
}

Piece *PieceTable::GetPiece(unsigned int docIndex, bool activate)
{
    if (ValidActivePiece(docIndex))
    {
        return &pieces[activePiece.idx];
    }
    else
    {
        unsigned int cumulativeLength = 0;
        unsigned int currentIdx = 0;
        for (auto &piece : pieces)
        {
            if (docIndex < cumulativeLength)
            {
                break;
            }
            unsigned int normalIndex = docIndex - cumulativeLength;
            if (normalIndex < piece.length)
            {
                if (activate)
                {
                    activePiece = {.cLen = cumulativeLength, .idx = currentIdx};
                }
                return &piece;
            }
            cumulativeLength += piece.length;
            currentIdx++;
        }
        if (pieces.empty() || docIndex == documentLength)
        {
            pieces.push_back({sourceType::add, static_cast<unsigned int>(addBuffer.size()), static_cast<unsigned int>(0)});
            if (activate)
            {
                activePiece = {.cLen = cumulativeLength, .idx = currentIdx};
            }
            return &pieces.back();
        }
        return nullptr;
    }
}

char PieceTable::GetCharAt(unsigned int docIndex) const
{
    if (docIndex >= documentLength)
    {
        return '\0';
    }
    unsigned int overallIndex = 0;
    for (const auto &piece : pieces)
    {
        if (docIndex < (piece.length + overallIndex) && docIndex >= overallIndex)
        {
            const string &buffer = piece.source == sourceType::original ? originalBuffer : addBuffer;
            return buffer[docIndex - overallIndex];
        }
        overallIndex += piece.length;
    }
    return '\0';
}

unsigned int horizMax = 0;

unsigned int PieceTable::MoveCursorX(unsigned int currOffset, int dir)
{
    int nexPos = currOffset + dir;
    if (dir < 0 && nexPos < 0)
    {
        return currOffset;
    }
    else if (dir >= 0 && nexPos > documentLength)
    {
        return currOffset;
    }

    if (!ValidActivePiece(currOffset))
    {
        out_of_range("Active Piece is OFF!");
    }

    if (nexPos < activePiece.cLen)
    {
        if (activePiece.idx - 1 < 0)
        {
            return currOffset;
        }
        // Go one piece back
        activePiece = {.cLen = activePiece.cLen - pieces[activePiece.idx - 1].length, .idx = activePiece.idx - 1};
    }

    if (nexPos > activePiece.cLen + pieces[activePiece.idx].length)
    {
        if (activePiece.idx + 1 >= pieces.size())
        {
            return currOffset;
        }
        // Go one piece forward
        activePiece = {.cLen = activePiece.cLen + pieces[activePiece.idx].length, .idx = activePiece.idx + 1};
    }

    // Bounding horizontal Maximum to line limits
    unsigned int currentLineIndex = std::upper_bound(lineStarts.begin(), lineStarts.end(), currOffset) - lineStarts.begin();

    if (currentLineIndex > 0)
    {
        currentLineIndex--;
    }

    unsigned int currentLineStart = lineStarts[currentLineIndex];
    unsigned int currentLineEnd = (currentLineIndex + 1 < lineStarts.size())
                                      ? lineStarts[currentLineIndex + 1]
                                      : documentLength;
    int newOffset = static_cast<int>(currOffset) + dir;

    if (newOffset < static_cast<int>(currentLineStart))
    {
        newOffset = currentLineStart;
    }
    else if (newOffset >= static_cast<int>(currentLineEnd))
    {
        newOffset = currentLineEnd - 1;
    }

    unsigned int newColumn = newOffset - currentLineStart;
    horizMax = newColumn;
    return currOffset + dir;
}

unsigned int PieceTable::MoveCursorY(unsigned int currOffset, int dir)
{
    if (lineStarts.empty())
    {
        return currOffset;
    }
    auto it = std::upper_bound(lineStarts.begin(), lineStarts.end(), currOffset);
    unsigned int currIndex = (it == lineStarts.begin())
                                 ? 0
                                 : std::distance(lineStarts.begin(), it) - 1;
    unsigned int newLineIndex;
    if (dir < 0)
    {
        if (currIndex >= lineStarts.size() - 1)
        {
            return documentLength;
        }
        newLineIndex = currIndex + 1;
    }
    else
    {
        if (currIndex == 0)
        {
            return 0;
        }
        newLineIndex = currIndex - 1;
    }

    unsigned int newLineStart = lineStarts[newLineIndex];
    unsigned int newLineEnd = (newLineIndex < lineStarts.size() - 1)
                                  ? lineStarts[newLineIndex + 1]
                                  : documentLength;
    unsigned int newLineLength = newLineEnd - newLineStart;
    return newLineStart + std::min(horizMax, newLineLength);
}

void PieceTable::Visualize()
{
    if (!DEBUG)
        return;
    const int width = 60; // Total width of visualization

    if (pieces.empty())
    {
        return;
    }

    // Ensure the activePiece.idx is within bounds
    if (activePiece.idx >= pieces.size())
    {
        cout << "Error: Invalid activePiece.idx (" << activePiece.idx << ")." << endl;
        return;
    }

    Piece *currentPiece = &pieces[activePiece.idx]; // Current piece pointer
    cout << string(width, '=') << endl;
    cout << "Piece Table Visualization (Document Length: " << documentLength << ")" << endl;
    cout << string(width, '=') << endl;

    // Header
    cout << left
         << setw(8) << "Index"
         << setw(10) << "Source"
         << setw(10) << "Start"
         << setw(10) << "Length" << endl;

    // Print each piece
    for (size_t i = 0; i < pieces.size(); i++)
    {
        const Piece &piece = pieces[i];

        // Highlight current piece
        if (&piece == currentPiece)
        {
            cout << "\033[1;32m"; // Green text for current piece
        }

        cout << left
             << setw(8) << i
             << setw(10) << (piece.source == sourceType::original ? "Original" : "Added")
             << setw(10) << piece.start
             << setw(10) << piece.length;

        if (&piece == currentPiece)
        {
            cout << " (Current)";
            cout << "\033[0m"; // Reset color
        }

        cout << endl;
    }

    // Visual representation of pieces
    cout << string(width, '-') << endl;
    cout << "Visual Map:" << endl;

    unsigned int currentPos = 0;
    string visualMap(width, ' ');

    for (size_t i = 0; i < pieces.size(); i++)
    {
        const Piece &piece = pieces[i];

        // Calculate piece's visual width proportional to its length
        double ratio = static_cast<double>(piece.length) / documentLength;
        int pieceWidth = max(1, static_cast<int>(ratio * (width - 2)));

        // Fill the visual map
        char fillChar = (piece.source == sourceType::original) ? 'O' : 'A';
        for (int j = 0; j < pieceWidth && currentPos < width; j++)
        {
            visualMap[currentPos++] = fillChar;
        }
    }

    cout << '[' << visualMap << ']' << endl;
    cout << " O = Original text, A = Added text" << endl;

    // Print first few line starts (if any)
    cout << "[ ";
    for (unsigned int lineStart : lineStarts)
    {
        cout << lineStart << " ";
    }
    cout << "]" << endl;

    // Active Piece section (new addition)
    cout << string(width, '-') << endl;
    cout << "Active Piece (Index: " << activePiece.idx << "):" << endl;
    cout << left
         << setw(8) << "Index"
         << setw(10) << "Source"
         << setw(10) << "Start"
         << setw(10) << "CLen" << endl;
    cout << string(width, '-') << endl;

    // Print active piece info
    const Piece &active = pieces[activePiece.idx];
    cout << left
         << setw(8) << activePiece.idx
         << setw(10) << (active.source == sourceType::original ? "Original" : "Added")
         << setw(10) << active.start
         << setw(10) << activePiece.cLen << endl;

    // Print some statistics
    cout << "Statistics:" << endl;
    cout << "- Total pieces: " << pieces.size() << endl;
    cout << "- Document length: " << documentLength << " characters" << endl;
    cout << "- Last deleted index: " << lastDeletedIndex << endl;
    cout << string(width, '=') << endl;
}
