#pragma once
#ifndef PIECETABLE_H
#define PIECETABLE_H

#include <vector>
#include <string>

enum sourceType
{
    original,
    add
};

struct Piece
{
    unsigned int start;
    unsigned int length;
    const sourceType source;
    std::vector<unsigned int> lineStart;
};

class PieceTable
{
public:
    PieceTable();
    PieceTable(const std::string &originalText);
    void Insert(unsigned int index, const std::string &text);
    void Delete(unsigned int index, unsigned int length);
    std::string getContent() const;
    char *getCharAt(unsigned int index) const;

private:
    std::string originalBuffer;
    std::string addBuffer;
    std::vector<Piece> pieces;
};

#endif