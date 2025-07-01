#ifndef TTF_READER_H
#define TTF_READER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// TTF Header (Offset Table)
struct TTFHeader {
    uint32_t scalerType;
    uint16_t numTables;
    uint16_t searchRange;
    uint16_t entrySelector;
    uint16_t rangeShift;
};

struct GlyphHeader {
    int16_t numberOfContours;
    int16_t xMin, yMin, xMax, yMax;
};

struct TTFPoint {
    int16_t x, y;
    bool onCurve;
};

struct SimpleGlyph {
    GlyphHeader header;
    std::vector<uint16_t> endPtsOfContours;
    std::vector<TTFPoint> points;
};

// Table Directory Entry
struct TableEntry {
    char tag[5];       
    uint32_t checksum;
    uint32_t offset;
    uint32_t length;
};

struct BezierPoint {
    float x, y;
    BezierPoint(float x = 0, float y = 0) : x(x), y(y) {}
};

// cmap table structures
struct CmapSubtable {
    uint16_t platformID;
    uint16_t encodingID;
    uint32_t offset;
};

struct CmapHeader {
    uint16_t version;
    uint16_t numTables;
    std::vector<CmapSubtable> subtables;
};

struct CmapFormat4 {
    uint16_t format;
    uint16_t length;
    uint16_t language;
    uint16_t segCountX2;
    uint16_t searchRange;
    uint16_t entrySelector;
    uint16_t rangeShift;
    std::vector<uint16_t> endCode;
    uint16_t reservedPad;
    std::vector<uint16_t> startCode;
    std::vector<int16_t> idDelta;
    std::vector<uint16_t> idRangeOffset;
    std::vector<uint16_t> glyphIdArray;
};

class TTFReader {
private:
    std::ifstream file;
    bool littleEndian;
    bool isLongFormat = false; 
    std::vector<uint32_t> glyphOffsets;
    
    // Font metrics
    uint16_t unitsPerEm = 1000; // Default value, will be read from head table
    
    // cmap table data
    uint32_t cmapTableOffset = 0;
    bool cmapLoaded = false;
    CmapHeader cmapHeader;
    CmapFormat4 cmapFormat4;
    
    // Byte swapping utilities
    uint16_t swapUint16(uint16_t val);
    uint32_t swapUint32(uint32_t val);
    bool isLittleEndian();

public:
    TTFReader();
    ~TTFReader();
    
    bool openFont(const std::string& filename);
    void close();
    
    bool readHeader(TTFHeader& header);
    bool readTableEntry(TableEntry& entry);
    void printHeader(const TTFHeader& header);
    void printTableEntry(const TableEntry& entry);

    bool findTable(const std::string& tableName, TableEntry& entry);
    bool seekToTable(const std::string& tableName);
    std::vector<uint8_t> readBytes(size_t count);
    void printHexDump(const std::vector<uint8_t>& data, size_t offset = 0);

    // glphy reading functions
    bool readGlyphHeader(GlyphHeader& header);
    bool readSimpleGlyph(SimpleGlyph& glyph);
    void printGlyph(const SimpleGlyph& glyph);
    void plotGlyph(const SimpleGlyph& glyph, int width = 80, int height = 40);
    void exportGlyphSVG(const SimpleGlyph& glyph, const std::string& filename);
    bool readMultipleGlyphs(int count);
    void explainLocaTable();
    bool loadLocaTable(); 
    bool readGlyphByIndex(int glyphIndex, SimpleGlyph& glyph);
    void readMultipleGlyphsByIndex(int startIndex, int count);

    // Bezier curve functions
    BezierPoint lerp(const BezierPoint& p1, const BezierPoint& p2, float t);
    BezierPoint quadraticBezier(const BezierPoint& start, const BezierPoint& control, const BezierPoint& end, float t);
    std::vector<BezierPoint> generateBezierCurve(const BezierPoint& start, const BezierPoint& control, const BezierPoint& end, int resolution = 20);
    std::vector<BezierPoint> generateGlyphOutline(const SimpleGlyph& glyph, int resolution = 20);
    std::vector<BezierPoint> generateContourOutline(const SimpleGlyph& glyph, size_t startPt, size_t endPt, int resolution = 20);
    void exportGlyphWithCurves(const SimpleGlyph& glyph, const std::string& filename);
    
    // cmap table functions
    bool loadCmapTable();
    bool parseCmapFormat4(uint32_t subtableOffset);
    uint16_t getGlyphIndex(uint32_t unicode);
    void printCmapInfo();
    
    // Font metrics functions
    bool loadHeadTable();
    uint16_t getUnitsPerEm() const { return unitsPerEm; }
};

#endif