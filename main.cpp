#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <algorithm>

bool FileRead(char* buff, FILE* fp);

bool StrDivide(char* buff, char* text[], int* CharNum, int StrNum);

bool ComparatorStr(const void* str1, const void* str2);

bool PrintText(char** text, FILE* f, int NumStr);

bool SpeedStrSort(char** text, int first, int second, bool (*cmp)(const void*, const void*));

bool ReverseComparatorStr(const void* str1, const void* str2);

bool ReverseStr(char* text[], char* rtext[], int StrNum, int CharNum);

bool NormalizeReverseText(char* text[], char* first, int StrNum);

bool OriginalPrint(char* buff, FILE* f, int StrNum);

int main()
{
    int CharSize = 0;

    FILE *f = fopen("hamlet.txt", "r");
    fseek(f, 0L, SEEK_END);
    CharSize = ftell(f);
    rewind(f);

    char* buffer = (char*)calloc ( (CharSize + 1), sizeof(char));
    assert (buffer != NULL);

    FileRead(buffer, f);

    int StrNum = 0;
    char* BuffTmp = buffer;

    for (int i = 0; i < CharSize + 1; ++i)
    {
        if(*BuffTmp == '\n')
        {
            *BuffTmp = '\0';
            ++StrNum;
        }
        ++BuffTmp;
    }

    char* text[StrNum];
    char* rtext[StrNum];

    StrDivide(buffer, text, &CharSize, StrNum);

    ReverseStr(text, rtext, StrNum, CharSize);

    fclose(f);

    SpeedStrSort(text, 0, StrNum-1, ComparatorStr);
    SpeedStrSort(rtext, 0, StrNum-1, ReverseComparatorStr);
    f = fopen("dictionary_of_english_life.txt", "w+");

    fprintf(f,"Dictionary of english life:\n");
    PrintText(text, f, StrNum);
    fprintf(f,"\n\nRap Dictionary:\n");
    NormalizeReverseText(rtext, buffer, StrNum);
    PrintText(rtext, f, StrNum);
    fprintf(f,"\n\nOriginal:\n");
    OriginalPrint(buffer, f, StrNum);

    fclose(f);
    free(buffer);

    return 0;
}

bool SpeedStrSort(char** text, int first, int second, bool (*cmp)(const void*, const void*))
{
    assert(text != NULL);
    assert(cmp != NULL);

    int NowStr = second;
    char* Tmp;
    int i = first;

    while (i < NowStr)
    {
        if( cmp(text[NowStr], text[i]) )
        {
            Tmp = text[NowStr];
            text[NowStr] = text[(NowStr - 1)];
            text[(NowStr - 1)] = Tmp;
            NowStr = NowStr - 1;
            if(i != (NowStr))
            {
                Tmp = text[i];
                text[i] = text[(NowStr + 1)];
                text[(NowStr + 1)] = Tmp;
            }
        } else
        {
            ++i;
        }
    }
    if (first < second)
    {
        SpeedStrSort(text, first, (NowStr - 1), cmp);
        SpeedStrSort(text, (NowStr + 1), second, cmp);
    }
}

bool FileRead(char* buff, FILE* fp)
{
    assert (buff != NULL);
    assert (fp != NULL);

    char* LastChar = buff;

    while ( (*LastChar = getc (fp)) != EOF )
    {
        ++LastChar;
    }
    LastChar = '\0';
    return 1;
}

bool StrDivide(char* buff, char* text[], int* CharNum, int StrNum)
{
    assert (buff != NULL);
    assert (text != NULL);

    int NowStr = 1;
    text[0] = buff;
    for (int i = 1;i < *CharNum; ++i)
    {
        if( *(buff + i) == '\0' && StrNum > NowStr)
        {
            text[NowStr] = buff + i + 1;
            ++NowStr;
        }
    }
    return 1;
}

bool ComparatorStr(const void* str1, const void* str2)
{
    assert((char*)str1 != NULL);
    assert((char*)str2 != NULL);

    int NowPos1 = 0;
    int NowPos2 = 0;
    while( ( *((char*)str1 + NowPos1) == *((char*)str2 + NowPos2) ) && (*((char*)str1 + NowPos1 + 1) != '\0' && *((char*)str2 + NowPos2 + 1) != '\0') )
    {
        if(*((char*)str1 + NowPos1 + 1) == ' ')
        {
            ++NowPos1;
        }
        if(*((char*)str2 + NowPos2 + 1) == ' ')
        {
            ++NowPos2;
        }
        ++NowPos1;
        ++NowPos2;
    }
    return *((char*)str1 + NowPos1) < *((char*)str2 + NowPos2);
}

bool PrintText(char* text[], FILE* f, int NumStr)
{
    assert(text != NULL);
    assert(f != NULL);

    int tmp = 0;
    int NowStr = 0;

    while (NowStr < NumStr)
    {
        tmp = 0;

        while (*(text[NowStr] + tmp) != '\0')
        {
            fprintf(f,"%c", *(text[NowStr] + tmp));
            ++tmp;
        }
        ++NowStr;
        fprintf(f,"\n");
    }

    return 0;
}

bool ReverseComparatorStr(const void* str1, const void* str2)
{
    assert((char*)str1 != NULL);
    assert((char*)str2 != NULL);

    int NowPos1 = 0;
    int NowPos2 = 0;
    while( ( *((char*)str1 - NowPos1) == *((char*)str2 - NowPos2) ) && (*((char*)str1 - (NowPos1 + 1) ) != '\0' && *((char*)str2 - (NowPos2 + 1) ) != '\0') )
    {
        if(*((char*)str1 - (NowPos1 + 1) ) == ' ')
        {
            ++NowPos1;
        }
        if(*((char*)str2 - (NowPos2 + 1) ) == ' ')
        {
            ++NowPos2;
        }
        ++NowPos1;
        ++NowPos2;
    }
    return *((char*)str1 - NowPos1) < *((char*)str2 - NowPos2);
}

bool ReverseStr(char* text[], char* rtext[], int StrNum, int CharNum)
{
    assert(text != NULL);
    assert(rtext != NULL);
    for (int i = 0; i < (StrNum - 1); ++i)
    {
        rtext[i] = (char*)((int)text[i + 1] - 2);
    }
    rtext[StrNum - 1] = (char*)((int)text[0] + CharNum - 2);

    return 0;
}

bool NormalizeReverseText(char* text[], char* first, int StrNum)
{
    assert(text != NULL);
    assert(first != NULL);

    for (int i = 0; i < StrNum; ++i)
    {
        while ( *(text[i] - 2) != '\0' && (text[i] - 1) != first)
        {
            --text[i];
        }
        --text[i];
    }
    return 0;
}

bool OriginalPrint(char* buff, FILE* f, int StrNum)
{
    assert(f != NULL);
    assert(buff != NULL);

    char* tmp = buff;
    int i = 0;
    while (i < StrNum)
    {
        fprintf(f, "%c", *tmp);
        ++tmp;

        if(*tmp == '\0' || *tmp == '\n')
        {
            ++i;
            fprintf(f, "\n");
            ++tmp;
        }
    }
    return 0;
}
