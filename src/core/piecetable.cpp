#include "piecetable.h"
#include <iostream>
#include <iomanip>

using namespace std;

bool DEBUG = false;

PieceTable::PieceTable() : PieceTable("") {}

PieceTable::PieceTable(const string &initialContent)
{
    const unsigned int bufferSize = initialContent.size();
    vector<unsigned int> lineStarts = GetLineStarts(sourceType::original, 0, bufferSize);
    if (bufferSize > 0)
    {
        originalBuffer = initialContent;
        pieces.push_back({sourceType::original, 0, static_cast<unsigned int>(bufferSize), lineStarts});
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
    vector<unsigned int> lineStarts = GetLineStarts(sourceType::add, index, tsize);

    unsigned int normalizedIdx = index - activePiece.cLen;
    // Appending Case
    if (normalizedIdx == currentPiece->length || currentPiece->length == 0)
    {
        if (currentPiece->source == original || (lastDeletedIndex >= currentPiece->start && lastDeletedIndex < currentPiece->start + currentPiece->length))
        {
            // TODO: FIX/REMOVE THIS ORIGINAL BUFFER CASE
            out_of_range("Delete Case Triggered");
            pieces.push_back({sourceType::add, documentLength, tsize, lineStarts});
            activePiece = {.idx = activePiece.idx + 1, .cLen = activePiece.cLen + currentPiece->length};
        }
        else
        {
            currentPiece->length += tsize;
            currentPiece->lineStart.insert(
                currentPiece->lineStart.end(),
                lineStarts.begin(),
                lineStarts.end());
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
                           normalizedIdx,
                           GetLineStarts(preSplitPiece.source, preSplitPiece.start, normalizedIdx)};
        currIter = pieces.erase(currIter);
        currIter = pieces.insert(currIter, prevPiece);
        ++currIter;
    }
    Piece middlePiece = {sourceType::add, static_cast<unsigned int>(addBuffer.size()) - tsize, tsize, lineStarts};
    currIter = pieces.insert(currIter, middlePiece);
    activePiece = {.cLen = activePiece.cLen + normalizedIdx, .idx = normalizedIdx == 0 ? activePiece.idx : activePiece.idx + 1};

    if (normalizedIdx > 0 && normalizedIdx < preSplitPiece.length)
    {
        unsigned int newStart = preSplitPiece.start + normalizedIdx;
        Piece nextPiece = {preSplitPiece.source,
                           newStart,
                           preSplitPiece.length - normalizedIdx,
                           GetLineStarts(preSplitPiece.source, newStart, preSplitPiece.length - normalizedIdx)};
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

string PieceTable::GetContent() const
{
    string content;
    for (const auto &piece : pieces)
    {
        const string &buffer = piece.source == sourceType::original ? originalBuffer : addBuffer;
        content += buffer.substr(piece.start, piece.length);
    }
    return content;
}

bool PieceTable::ValidActivePiece(unsigned int index)
{
    if (activePiece.idx < pieces.size())
    {
        Piece *currentPiece = &pieces[activePiece.idx];
        if (index < activePiece.cLen)
        {
            return false;
        }
        unsigned int normalizedIdx = index - activePiece.cLen;
        if (normalizedIdx >= 0 && normalizedIdx <= currentPiece->length)
        {
            return true;
        }
    }
    return false;
}

Piece *PieceTable::GetActivePiece(unsigned int index)
{
    if (ValidActivePiece(index))
    {
        return &pieces[activePiece.idx];
    }
    else
    {
        unsigned int cumulativeLength = 0;
        unsigned int currentIdx = 0;
        for (auto &piece : pieces)
        {
            if (index < cumulativeLength)
            {
                break;
            }
            unsigned int normalIndex = index - cumulativeLength;
            if (normalIndex < piece.length)
            {
                activePiece = {.cLen = cumulativeLength, .idx = currentIdx};
                return &piece;
            }
            cumulativeLength += piece.length;
            currentIdx++;
        }
        if (pieces.empty() || index == documentLength)
        {
            pieces.push_back({sourceType::add, static_cast<unsigned int>(addBuffer.size()), static_cast<unsigned int>(0), {}});
            activePiece = {.cLen = cumulativeLength, .idx = currentIdx};
            return &pieces.back();
        }
        return nullptr;
    }
}

char PieceTable::GetCharAt(unsigned int index) const
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

vector<unsigned int> PieceTable::GetLineStarts(sourceType src, unsigned int start, unsigned int len) const
{
    vector<unsigned int> lineStarts = {};
    // TODO: Make this work with original Buffer included
    return lineStarts;
    unsigned int end = start + len;
    if (start < 0 || end < 0 || start >= documentLength || end >= documentLength)
    {
        return lineStarts;
    }
    cout << "Type: " << (sourceType::add ? "Add" : "Original") << endl;
    cout << "Start: " << start << endl;
    cout << "End: " << end << endl;
    string text = src == sourceType::original
                      ? originalBuffer.substr(start, end)
                      : addBuffer.substr(start, end);
    for (unsigned int index = start; index < end; index++)
    {
        if (text[index] == '\n')
        {
            lineStarts.push_back(index + 1);
        }
    }
    return lineStarts;
}

void PieceTable::Visualize()
{
    if (!DEBUG)
        return;
    const int width = 80; // Total width of visualization

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
         << setw(10) << "Length"
         << "Line Starts" << endl;
    cout << string(width, '-') << endl;

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

        // Print first few line starts (if any)
        cout << "[ ";
        size_t shown = 0;
        for (size_t lineStart : piece.lineStart)
        {
            if (shown++ < 3)
            {
                cout << lineStart << " ";
            }
            else
            {
                cout << "... ";
                break;
            }
        }
        cout << "]";

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

    // Active Piece section (new addition)
    cout << string(width, '-') << endl;
    cout << "Active Piece (Index: " << activePiece.idx << "):" << endl;
    cout << left
         << setw(8) << "Index"
         << setw(10) << "Source"
         << setw(10) << "Start"
         << setw(10) << "CLen"
         << "Line Starts" << endl;
    cout << string(width, '-') << endl;

    // Print active piece info
    const Piece &active = pieces[activePiece.idx];
    cout << left
         << setw(8) << activePiece.idx
         << setw(10) << (active.source == sourceType::original ? "Original" : "Added")
         << setw(10) << active.start
         << setw(10) << activePiece.cLen;

    // Print line starts for the active piece
    cout << "[ ";
    size_t shown = 0;
    for (size_t lineStart : active.lineStart)
    {
        if (shown++ < 3)
        {
            cout << lineStart << " ";
        }
        else
        {
            cout << "... ";
            break;
        }
    }
    cout << "]" << endl;

    cout << string(width, '=') << endl;

    // Print some statistics
    cout << "Statistics:" << endl;
    cout << "- Total pieces: " << pieces.size() << endl;
    cout << "- Document length: " << documentLength << " characters" << endl;
    cout << "- Last deleted index: " << lastDeletedIndex << endl;
    cout << string(width, '=') << endl;
}
