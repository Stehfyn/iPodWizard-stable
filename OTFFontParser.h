#include <Afxtempl.h>

#pragma once

#define SWAPWORD(x) MAKEWORD(HIBYTE(x), LOBYTE(x))
#define SWAPLONG(x) MAKELONG(SWAPWORD(HIWORD(x)), SWAPWORD(LOWORD(x)))

//big endian classes
class BE_US
{
public:
	BE_US(USHORT data) : m_data(data) {}
protected:
	USHORT m_data;
public:
	void operator= (USHORT param)
	{
		m_data=SWAPWORD(param);
	}
	operator USHORT()
	{
		USHORT temp;
		temp=SWAPWORD(m_data);
		return temp;
	}
	bool operator!= (int param)
	{
		if (SWAPWORD(m_data) != param)
			return true;
		else
			return false;
	}
	bool operator== (int param)
	{
		if (SWAPWORD(m_data) == param)
			return true;
		else
			return false;
	}
	USHORT operator+ (int param)
	{
		USHORT temp=SWAPWORD(m_data)+param;
		return temp;
	}
	USHORT operator+ (BE_US param)
	{
		USHORT temp=SWAPWORD(m_data)+param;
		return temp;
	}
	USHORT operator- (int param)
	{
		USHORT temp=SWAPWORD(m_data)-param;
		return temp;
	}
	USHORT operator- (USHORT param)
	{
		USHORT temp=SWAPWORD(m_data)-param;
		return temp;
	}
	USHORT operator- (BE_US param)
	{
		USHORT temp=SWAPWORD(m_data)-param;
		return temp;
	}
};

class BE_UL
{
public:
	BE_UL(ULONG data) : m_data(data) {}
protected:
	ULONG m_data;
public:
	void operator= (ULONG param)
	{
		m_data=param;
	}
	operator ULONG()
	{
		ULONG temp;
		temp=SWAPLONG(m_data);
		return temp;
	}
	bool operator!= (int param)
	{
		if (SWAPLONG(m_data) != param)
			return true;
		else
			return false;
	}
	bool operator== (int param)
	{
		if (SWAPLONG(m_data) == param)
			return true;
		else
			return false;
	}
	ULONG operator+ (int param)
	{
		LONG temp=SWAPLONG(m_data)+param;
		return temp;
	}
	ULONG operator- (int param)
	{
		LONG temp=SWAPLONG(m_data)-param;
		return temp;
	}
};

//font structure
struct TABLES_HEADER
{
	//ULONG sfnt_version; //fixed 0x00010000
	BE_US uMajorVersion;
    BE_US uMinorVersion;
	BE_US  numTables;
	BE_US  searchRange;
	BE_US  entrySelector;
	BE_US  rangeShift;
};

struct TABLE_DIRECTORY
{
	char	name[4];
	BE_UL	checkSum;
	BE_UL	offset;
	BE_UL	length;
};

//cmap
struct CMAP_HEADER
{
	BE_US version; //=0
	BE_US numTables;
};

struct ENCODING_RECORD
{
	BE_US platformID;
	BE_US encodingID;
	BE_UL offset; //from begining of cmap table till the encoding subtable
};

//encoding formats
struct ENC_FORMAT0
{
	BE_US format;
	BE_US length;
	BE_US language;
	BYTE glyphIdArray[256];
};

struct ENC_FORMAT4
{
	BE_US format;
	BE_US length;
	BE_US language;
	//all calculations are made for us and stored in following variables:
	BE_US segCount; //segCountx2
	BE_US searchRange; //2 x (2**floor(log2(segCount)))
	BE_US entrySelector;
	BE_US rangeShift;

	//USHORT endCount[segCount]; //End characterCode for each segment, last=0xFFFF. 
	//USHORT reservedPad; //Set to 0. 
	//USHORT startCount[segCount]; //Start character code for each segment. 
	//SHORT idDelta[segCount]; //Delta for all character codes in segment. 
	//USHORT idRangeOffset[segCount]; //Offsets into glyphIdArray or 0 
	//USHORT glyphIdArray[ ]; //Glyph index array (arbitrary length) 
};

//EBDT - stored the bitmap data of each glyph
struct EBDT_HEADER
{
	BE_UL version; //fixed
};

//EBLC

struct EBLC_HEADER
{
	BE_UL version; //initially defined as 0x00020000
	BE_UL numSizes; //Number of bitmapSizeTables
};

struct bitmapSizeTable
{
	BE_UL indexSubTableArrayOffset; //offset to index subtable from beginning of EBLC. 
	BE_UL indexTablesSize; //number of bytes in corresponding index subtables and array 
	BE_UL numberOfIndexSubTables; //an index subtable for each range or format change 
	BE_UL colorRef; //not used; set to 0. 
	//hori line metrics for text rendered horizontally 
	//vert line metrics for text rendered vertically 
	struct sBitLineMetrics
	{
		CHAR ascender;
		CHAR descender;
		BYTE widthMax;
		CHAR caretSlopeNumerator;
		CHAR caretSlopeDenominator;
		CHAR caretOffset;
		CHAR minOriginSB;
		CHAR minAdvanceSB;
		CHAR maxBeforeBL;
		CHAR minAfterBL;
		CHAR pad1;
		CHAR pad2;
	} hori, vert;
	BE_US startGlyphIndex; //lowest glyph index for this size 
	BE_US endGlyphIndex; //highest glyph index for this size 
	BYTE ppemX; //horizontal pixels per Em 
	BYTE ppemY; //vertical pixels per Em 
	BYTE bitDepth; //the Microsoft rasterizer v.1.7 or greater supports the following bitDepth values, as described below: 1, 2, 4, and 8. 
	CHAR flags; //vertical or horizontal (see bitmapFlags) 
};

struct indexSubTableArray
{
	BE_US firstGlyphIndex; //first glyph code of this range 
	BE_US lastGlyphIndex; //last glyph code of this range (inclusive) 
	BE_UL additionalOffsetToIndexSubtable; //add to indexSubTableArrayOffset to get offset from beginning of 'EBLC' 
};

struct NameRecord
{
	BE_US platformID; //Platform ID. 
	BE_US encodingID; //Platform-specific encoding ID. 
	BE_US languageID; //Language ID. 
	BE_US nameID; //Name ID. 
	BE_US length; //String length (in bytes). 
	BE_US offset; //String offset from start of storage area (in bytes). 
};

struct NAME_HEADER
{
	BE_US format; //Format selector (=0). 
	BE_US count; //Number of name records. 
	BE_US stringOffset; //Offset to start of string storage (from start of table). 
	//NameRecord nameRecord[count]; //The name records where count is the number of records. 
	//(Variable)  Storage for the actual string data. 
};

struct smallGlyphMetrics
{
	BYTE height;
	BYTE width;
	CHAR BearingX;
	CHAR BearingY;
	BYTE Advance;
};


//saves the info we need about the font file:
struct GLYPH_INFO
{
	DWORD offset; //image data offset from begining of EBDT table
	WORD format; //the format of the image
	WORD index; //glyph index
	DWORD size; //size of data
	BYTE width;
	BYTE height;
	CHAR bearingX;
	CHAR bearingY;
	BYTE Advance;
};

struct UNICODE_GROUP
{
	WORD	startOffset;
	WORD	endOffset;
	WORD	format;
	USHORT	*byteoffset_s; //start count
	USHORT	*byteoffset_e; //end count
	SHORT	*byteoffset_d; //delta
	USHORT	*byteoffset_r; //rangeoffset
	CWordArray	glyphIndex;
};

struct GLYPH_OFFSETS
{
	WORD format;
	LPBYTE offset_start;
	ULONG ebdt_offset;
	USHORT first;
	ULONG final_offset;
};


//Font class
class COTFFont
{
// construction
public:
	COTFFont(void);
	~COTFFont(void);

// implementation
private:
	TABLES_HEADER	*			m_pHeader;
	CMAP_HEADER		*			m_pCMap;
	EBDT_HEADER		*			m_pEBDT;
	EBLC_HEADER		*			m_pEBLC;
	NAME_HEADER		*			m_pName;
	GLYPH_OFFSETS				m_GlyphsOffsets;

	TABLE_DIRECTORY *			m_CMapChecksum;
	TABLE_DIRECTORY *			m_EBDTChecksum;
	TABLE_DIRECTORY *			m_EBLCChecksum;
	
	CString		m_FontName;
	CString		m_FontStyle;
	WORD		m_BitDepth;
	WORD		m_FontSize;
	DWORD		m_BigLen;
	DWORD		m_EBDTLen;
	DWORD		m_Offset;
	CArray<GLYPH_INFO*,GLYPH_INFO*> m_Glyphs;
	CArray<UNICODE_GROUP*,UNICODE_GROUP*> m_UnicodeGroups;
	CWordArray	m_GlyphTable;
	CWordArray	m_UnicodeGlyphs;

// operations
public:
	BOOL	Read(LPBYTE lpPos, BOOL bParse = TRUE);
	BOOL	ParseCMap();
	BOOL	ParseEBDT();
	BOOL	ParseEBLC();
	BOOL	ParseName();

	COLORREF	GetFontPixel(WORD index, CPoint point);
	void		SetFontPixel(WORD index, LONG x, LONG y, COLORREF color);
	LPCTSTR		GetFontName();
	LPCTSTR		GetFontStyle();
	DWORD		GetFontBlockLen();
	WORD		GetFontBitDepth();
	WORD		GetFontSize();
	WORD		GetNumGlyphs();
	WORD		GetNumUnicodeGroups();
	void		GetUnicodeGroup(WORD index, LPWORD start, LPWORD end, LPWORD offset);
	WORD		GetUnicodeChar(WORD index, LPWORD groupIndex);
	WORD		GetNumUnicodeChars();
	WORD		GetUnicodeCharOffset(WORD c);
	void		GetCharMetrics(WORD index, LPBYTE width, LPBYTE height, CHAR *bearingx, CHAR *bearingy, LPBYTE advance);
	WORD		GetGlyphIndex(WORD index);
	CSize		GetFontBitmapSize(WORD index);
	void		SetCharMetrics(WORD index, BYTE width, BYTE height, CHAR bearingx, CHAR bearingy, BYTE advance, BOOL bMakeFont = FALSE);
	void		SetUnicodeGroup(WORD index, WORD start, WORD end, WORD offset);
	BOOL		GetUnicodeGroupGlyphState(WORD index);
	DWORD		GetGlyphDataLen();
	void		ResetGlyphData();
	void		AddCharMetrics(WORD index, BYTE width, BYTE height, CHAR bearingx, CHAR bearingy, BYTE advance);
	void		FillEndGlyphs(int i);
	void		SyncChecksums();
};