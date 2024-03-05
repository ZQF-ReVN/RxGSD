## GSD Engine Games

| Date       | Developer                                | Title                                                        | Type | Version |
| :--------- | :--------------------------------------- | :----------------------------------------------------------- | :--: | :-----: |
| 2003-06-06 | [BLACKRAINBOW](https://vndb.org/p175)    | [催眠学園 / 催眠学園DVD with 催眠術](https://vndb.org/v1104) | PKG  |  1.0β   |
|            | Milk Pai                                 | Gカップ敏感聖徒会長～乳フェチ・乳揉み編～                    |      |         |
|            | Milk Pai                                 | Iカップ母乳看護婦 ～乳フェチ・乳吸い編～                     |      |         |
|            | Terios                                   | 夏色☆こみゅにけ～しょん♪                                     |      |         |
| 2004-02-27 | [GIGA](https://vndb.org/p119)            | [MOEラブ～なついろ萌黄寮～](https://vndb.org/v4935)          | PKG  | 2.1.0.8 |
| 2004-11-26 | [GIGA](https://vndb.org/p119)            | [やきもちツインベル](https://vndb.org/v4634)                 | PKG  | 2.1.3.8 |
| 2005-11-25 | [Fizz](https://vndb.org/p291)            | [恋もも](https://vndb.org/v3074)                             | PKG  | 2.1.9.0 |
| 2006-11-10 | [Daisy](https://vndb.org/p1933)          | [Always ～ふと、気が付けばキミとの日常…～](https://vndb.org/v1650) | PKG  | Unknow  |
| 2007-03-23 | [Fizz](https://vndb.org/p291)            | [ましろぼたん](https://vndb.org/v553)                        | PKG  | 2.2.1.0 |
| 2007-08-24 | [Fizz](https://vndb.org/p291)            | [ましろぼたん ミニファンディスク ～ゆかなんのホワイトバレンタイン～](https://vndb.org/v554) | PKG  | 2.2.1.0 |
| 2008-04-25 | [Fizz](https://vndb.org/p291)            | [朝凪のアクアノーツ](https://vndb.org/v1733)                 | PKG  | 2.3.0.0 |
| 2009-10-30 | [Fizz](https://vndb.org/p291)            | [さくらテイル](https://vndb.org/v1380)                       | PKG  | 3.0.1.4 |
| 2010-03-26 | [Atelier Sakura](https://vndb.org/p1224) | [他の男の精液で孕んでもいいですか…？ 女子校生寝取られ事情](https://vndb.org/v3787) | PKG  | 3.0.1.6 |
| 2010-06-25 | [Atelier Sakura](https://vndb.org/p1224) | [他の男の精液で孕んでもいいですか…？2 幼く見えるカラダは、既に男の味を知っていた](https://vndb.org/v4135) | PKG  | 3.0.1.6 |
| 2010-10-29 | [Atelier Sakura](https://vndb.org/p1224) | [他の男の精液で孕んでもいいですか…？3 僕の知らない所でイキ狂っていた最愛彼女―生真面目な学園副会長―](https://vndb.org/v5160) | PKG  | 3.0.1.8 |
| 2010-11-26 | [Atelier Sakura](https://vndb.org/p1224) | [他の男の精液で孕んでもいいですか…？4 憧れていた先輩のむっちり美乳は僕のものではなかった―知りたくなかった彼女の秘密―](https://vndb.org/v5292) | PKG  | 3.0.1.8 |
| 2010-11-26 | [Fizz](https://vndb.org/p291)            | [さくらのしっぽ ～さくらテイルファンディスク～](https://vndb.org/v5151) | PKG  | 3.0.2.5 |
| 2010-12-28 | [Pink Tissue](https://vndb.org/p1778)    | [人妻ンション～うわさの巨乳人妻三姉妹～](https://vndb.org/v5760) |  DL  | 3.0.2.6 |
| 2011-04-28 | [Atelier Sakura](https://vndb.org/p1224) | [清純なカラダは、アイツの腕の中で男を知っていく 彼女の幸せそうな姿を、指を咥えて見ているしかない物語](https://vndb.org/v5688) |  DL  | 3.0.2.6 |
| 2011-02-25 | [Atelier Sakura](https://vndb.org/p1224) | [ボクの目の前で親友に抱かれ腰を振る彼女 ―奴の言いなりにそのエロい肢体は開かれていく―](https://vndb.org/v5858) | PKG  | 3.0.2.9 |
| 2011-04-22 | [Atelier Sakura](https://vndb.org/p1224) | [愛する妻、玲奈の浮気告白『お、俺より気持ち良かったのか…？』→『…うん。凄かった』](https://vndb.org/v6755) | PKG  | 3.0.3.1 |



## Reference

https://github.com/shangjiaxuan/Crass-source/blob/4aff113b98fc39fb85f64501ab47c580df779a3d/cui-1.0.4/AGSD/AGSD.cpp



## Structure

```C
struct GSD_Pack_Info
{
	uint32_t uiFOA;
	uint32_t uiSize;
};

struct GSD_Pack_Entry
{
	GSD_Pack_Info Info;
	char aFileName[0x38];
};

struct GSD_Pack_HDR
{
	uint32_t uiEntryCount;
	GSD_Pack_Entry aEntry[uiEntryCount];
};

union GSD_STD_RawStr
{
  char *cpStr;
  char aStr[16];
};


struct GSD_STD_String
{
	GSD_STD_RawStr uStr;
	uint32_t uiLen;
	uint32_t uiReserve;
};

struct GSD_FS_Pack_Entry
{
	uint64_t uiFOA;
	uint64_t uiSize;
	char aFileName[0x70];
};

struct GSD_FS
{
  HANDLE hFile;
  uint32_t uiFilePointer_Back;
  uint32_t hFile_0;
  LARGE_INTEGER sOldFilePointer;
  uint32_t aUn0[3];
  LARGE_INTEGER sFilePointer;
  LARGE_INTEGER sFileSize;
  uint32_t hFile_1;
  uint32_t aUn1[28];
  uint32_t hFile_2;
  uint32_t aUn2[6];
  uint32_t hFile_3;
  uint32_t aUn3[6];
  uint32_t hFile_4;
  uint32_t aUn4[6];
  uint32_t uiSlot1_Handle;
  uint32_t uiSlot1_Un0;
  uint32_t uiSlot1_Handle_0;
  uint32_t aUn5[49];
  uint32_t pResSize;
  uint32_t nDecompressSize;
  uint32_t pResBuffer_Active;
  uint32_t uiEntryCount;
  uint32_t uiIsEntryMaxCount;
  uint32_t uiUnx;
  GSD_FS_Pack_Entry memEntry;
  uint8_t *pResBuffer_Org;
  uint32_t uiUnSize;
  uint32_t aUn6[52];
};

struct GSD_Spt
{
	uint8_t ucDecBegIndex;
	uint8_t ucType;
	uint8_t ucUn1;
	uint8_t ucUn2;
};

struct GSD_Script_Decoder_One
{
	uint32_t aKey0[4];
};

struct GSD_Script_Decoder
{
	GSD_Script_Decoder_One[16];
};

struct GSD_ANIM_Entry
{
	uin8_t aUn[0x58];
};

struct GSD_ANIM_HDR
{
	char aSignature[0x8]; // "ANIMV450"
	uint32_t uiEntryCount;
	uint32_t uiWidth;
	uint32_	 uiHeigh;
};

struct GSD_ANIM
{
	GSD_ANIM_Entry EntryList[HDR.uiEntryCount];
	GSD_ANIM_HDR HDR;
};

struct GSD_BMZ
{
  uint8_t aSignature[4]; // ZLC3
  uint32_t uiDecompressSize;
  // zlib compress data
  if(BMZ_Size - 0x8 == "ANIMV450")
  {
	GSD_ANIM ANIM
  }
};

```



### SPT

```C
struct SPT_Code_Append_Data_Type0 // Test / Msg / Voice
{
  uint32_t uiNameReallySeq; // char name seq in global.dat. seq str -> "歩未"
  uint32_t uiNameDisplaySeq; // char name seq in global.dat. seq str -> "???" (show this txt)
  uint32_t uiUn2;
  uint32_t m_uiVoiceFileSeq; // 0x00002711 -> 10001.ogg
  uint32_t uiCharCount;
  uint32_t uiStrType0Len;
  uint32_t uiStrType1Len;
  SPT_Char_Entry aCharIndex[uiCharCount]
  char aStrType0[uiStrType0Len + 1]
  char aStrType0[uiStrType1Len + 1]
};

// if str_len == 0 {size = 0x1D} else {size = 0x1D + strlen + 1}
struct SPT_Code_Append_Data_Type1_Ele
{
  uint32_t uiVal_0;
  uint32_t uiVal_1;
  uint32_t uiVal_2;
  uint32_t uiVal_3;
  uint32_t uiStrLen; // Val_4
  uint32_t uiVal_5;
  uint8_t uiVal_6;
  uint32_t uiVal_7;
  char aStr[uiStrLen + 1];
};

struct SPT_Code_Append_Data_Type2_Ele // Call Spt Function.like call load image or free.
{
  uint32_t uiType1Count;
  SPT_Code_Append_Type1 aAppendType1[uiType1Count];
};

struct SPT_Code_Append_Data_Type3_Ele
{
  uint32_t uiVal_0;
  uint32_t uiVal_1;
  uint32_t uiVal_2;
};

struct SPT_Code
{
  uint32_t uiCommand; // 0x00000022:Call Script, 0x00000001:Push Str, 
  uint32_t uiVal_1;
  uint32_t uiVal_2;
  uint32_t uiVal_3;
  uint32_t uiVal_4;
  uint32_t uiSequnece;
  uint32_t uiAppendDataCount_Type1;
  uint32_t uiAppendDataCount_Type2;
  uint32_t uiAppendDataCount_Type3;
  SPT_Code_Append_Data_Type0 aDataType0
  SPT_Code_Append_Data_Type1 aDataType1[uiAppendDataCount_Type1]
  SPT_Code_Append_Data_Type2 aDataType2[uiAppendDataCount_Type2]
  SPT_Code_Append_Data_Type3 aDataType3[uiAppendDataCount_Type3]
};

struct SPT_Append_Script_info
{
  uint32_t uiStrLen0;
  if uiStrLen0
  {
    char aScriptName[uiStrLen0];
  }
  uint32_t uiStrLen1;
  if uiStrLen1
  {
    char aScriptName[uiStrLen1];
  }
  uint32_t uiUn0;
  uint32_t uiUn1;
  uint32_t uiUn2;
  uint8_t aAppendData[0x80];
};

if uiScriptCount == 0 -> size = 4;
else size = 4 + SPT_Append_Script_info * uiScriptCount
struct SPT_Append_Script
{
  uint32_t uiScriptCount;
  if uiScriptCount
  {
    SPT_Append_Script_info Info[uiScriptCount]
  }
};

struct SPT_Encryptor_Info
{
    uint8_t ucDecStartIndex;
	uint8_t ucDecModeType;
	uint8_t ucUn0;
	uint8_t ucUn1;
};

struct SPT_Codes_Info
{
  uint32_t uiChunkCount;
  uint32_t uiUn0;
  uint32_t uiUn1;
  uint32_t uiUn2;
};

struct SPT
{
  SPT_Encryptor_Info Encryptor_Info;
  uint32_t uiUnFlag; // == 0
  uint32_t uiScriptNameLen;
  char aScriptName[uiScriptNameLen];
  SPT_Codes_Info ChunksInfo;
  SPT_Append_Script[0xF]
  uint32_t uiUnSize;
  SPT_Code[Block_Info.uiBlockCount]
};

struct Global_Dat_Str
{
    char aStr[260];
};

struct Global_Dat
{
    SPT_Encryptor_Info Encryptor_Info;
    uint32_t uiUnFlag; // == 0
    SPT_Append_Script[0xF];
    uint32_t uiGlobalStrCount;
    Global_Dat_Str aStrIndex[260];
    uint8_t aUnData[0x60];
};
```



## Tips

spt脚本可以不用加密回去，spt脚本开头有2个字节，
第一个字节是查找解密表的开始位置，但程序里有判断，大于等于8就不解密
第二个字节是第一次解密的模式，大于2就不解密
这两个字节都是xor了0xF0的，所以把开头的两个字节改成0xFF就可以跳过解密了

bmz图片其实就是 头四个字节，'ZLC3' 然后四个字节是解压后大小，后面就全是zlib的压缩数据
zib压缩数据解密后其实就是个bmp图片（完整的bmp文件结构）直接把解压后的数据保存，然后后缀改成bmp就可以查看了
也可以用zlib压缩回去，但没什么必要，因为程序里会判断ZLC3标识，即文件开头不是ZLC3就不会进行解压操作
所以直接把bmz解密后的bmp图片后缀改成bmz就可以了

spt脚本目前看来，很像是按照块来读取的，不过里面的结构很奇葩，有可能是直接从vector里拷贝出来的数据，其中有些数据看起来像是乱码的
估计是它这个加密的锅，但不影响正常读取，文本似乎可以直接按照结构变长，写了个序列化和反序化spt，不过重新序列化回去可以发现spt文件的大小都不一样了
观察发现，重新生成的spt文件和原始的spt文件数据是一样的，但原始的spt文件还多出来了一节数据，调试后发现，引擎其实也根本没用这些末尾的数据，看起来这些数据和垃圾数据一样，
反正随便测了一下，没什么问题，暂时先这样用着吧。

人物名在spt里不存储，spt里的文本结构里只存储人物名在global.dat中的字符串表的下标，所以改人物名要改global.dat，
这步做完人物名确实可以显示了，不过还会出现两个文件，第一个是原先不同人物对话的时候，字体会改变颜色，现在颜色变成了默认的
这个需要在color_font.spt里找到人物名的字符串改掉就好了，还有个问题是语音没了，
所以得在spt里搜索原先的人物名，全部改掉(text_init.spt,text_queue_wait.spt,text_second.spt)。
还有一些游戏内的字符串也得从spt文件里改，SPT_Text_Editor只提取文本框和选项框的文本