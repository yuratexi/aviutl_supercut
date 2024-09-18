#include <windows.h>
#include <filter.h>
#include <exedit.hpp>
#include <stdio.h>

#define M_ITEM_ALL_SPLIT (WM_USER + 0x1)
#define M_ITEM_CUSTOM1_SPLIT (WM_USER + 0x2)
#define M_ITEM_CUSTOM2_SPLIT (WM_USER + 0x3)
#define M_ITEM_CUSTOM3_SPLIT (WM_USER + 0x4)
#define M_ITEM_CUSTOM4_SPLIT (WM_USER + 0x5)
#define M_ITEM_CUSTOM5_SPLIT (WM_USER + 0x6)
#define M_ITEM_CUSTOM6_SPLIT (WM_USER + 0x7)
#define M_ITEM_CUSTOM7_SPLIT (WM_USER + 0x8)
#define M_ITEM_CUSTOM8_SPLIT (WM_USER + 0x9)
#define M_ITEM_CUSTOM9_SPLIT (WM_USER + 0xA)
#define M_ITEM_CUSTOM10_SPLIT (WM_USER + 0xB)

static const char all_split[] = ("分割ALL");
static const char custom1_split[] = ("分割カスタム1");
static const char custom2_split[] = ("分割カスタム2");
static const char custom3_split[] = ("分割カスタム3");
static const char custom4_split[] = ("分割カスタム4");
static const char custom5_split[] = ("分割カスタム5");
static const char custom6_split[] = ("分割カスタム6");
static const char custom7_split[] = ("分割カスタム7");
static const char custom8_split[] = ("分割カスタム8");
static const char custom9_split[] = ("分割カスタム9");
static const char custom10_split[] = ("分割カスタム10");


//constexpr TCHAR* track_name[] = { "1","2","3","4","5" };
//constexpr int    track_default[] = { 0,0,0,0,0 };

constexpr TCHAR* track_name[] = { "configファイルの設定方法を確認する","configファイルを開く"};
constexpr int    track_default[] = { -1,-1 };


FILTER_DLL filter = {
    FILTER_FLAG_ALWAYS_ACTIVE,
    NULL,NULL,
    const_cast<char*>("スーパーカット by ゆらてぃー"),
    NULL,NULL,NULL,
    NULL,NULL,
    2,          //チェックボックスの数
    const_cast<TCHAR**>(track_name),    //チェックボックス名前郡へのポインタ(名前が入ってる配列名を書く)
    const_cast<int*>(track_default),    //初期値へのポインタ
    NULL,
    NULL,
    NULL,
    NULL,
    func_WndProc,
};


EXTERN_C FILTER_DLL __declspec(dllexport)* __stdcall GetFilterTable(void) {
    return &filter;
}



FILTER* exeditfp;

EXFUNC exfunc;


FILTER* get_exeditfp(FILTER* fp) {
    SYS_INFO si;
    fp->exfunc->get_sys_info(NULL, &si);

    for (int i = 0; i < si.filter_n; i++) {
        FILTER* tfp = (FILTER*)fp->exfunc->get_filterp(i);
        if (tfp->information != NULL) {
            if (!strcmp(tfp->information, "拡張編集(exedit) version 0.92 by ＫＥＮくん")) return tfp;
        }
    }
    return NULL;
}

int* ObjectAlloc_ptr; // 0x1e0fa0
ExEdit::Object** ObjectArray_ptr; // 0x1e0fa4
int* SelectingObjectNum_ptr; // 0x167d88
int* SelectingObjectIndex; // 0x179230
int* split_mode; // 0x1538b0


static inline void(__cdecl* drawtimeline)(); // 39230



int select1[101] = { 0,1,2,3,-1 };
int select2[101] = { 0,1,2,3,-1 };
int select3[101] = { 0,1,2,3,-1 };
int select4[101] = { 0,1,2,3,-1 };
int select5[101] = { 0,1,2,3,-1 };
int select6[101] = { 0,1,2,3,-1 };
int select7[101] = { 0,1,2,3,-1 };
int select8[101] = { 0,1,2,3,-1 };
int select9[101] = { 0,1,2,3,-1 };
int select10[101] = { 0,1,2,3,-1 };



void item_all_split(void* editp, FILTER* fp) {
    int n = 0;
    int now_frame = fp->exfunc->get_frame(editp);    //現在のフレーム位置
    auto obj = *ObjectArray_ptr;

    //再生位置に存在するオブジェクトを全て選択
    for (int i = 0; i < *ObjectAlloc_ptr; ++i) {
        //オブジェクトが再生位置にあったら
        if (obj->frame_begin <= now_frame && now_frame <= obj->frame_end) {
            SelectingObjectIndex[n] = i;
            n++;
        }
        obj++;
        if ((int)obj->flag == 0) break; //flagが0だったら終端なので処理を終了する
    }

    *SelectingObjectNum_ptr = n;    //選択した数を代入
    
    int split_mode_org = *split_mode;   //org:originalの略
    *split_mode = 1;
    SendMessageA(exeditfp->hwnd, WM_COMMAND, 1051, -1); //"選択したオブジェクト"をカットする


    *split_mode = split_mode_org;

    *SelectingObjectNum_ptr = 0;
    drawtimeline(); //選択解除
}

//selectの添字が101なのは、レイヤーの数(100)+終端文字用(1)だから
void item_custom_split(void* editp, FILTER* fp,int select[101]) {
    int select_len = 0; //配列selectの長さ

    bool findflg = false;
    int n = 0;
    int now_frame = fp->exfunc->get_frame(editp);    //現在のフレーム位置

    auto obj = *ObjectArray_ptr;

    
    //select_lenを求める
    while (select[select_len] != -1) {
        select_len++;
    }

    //*ObjectAlloc_ptr=データの個数(大体)
    for (int i = 0; i < *ObjectAlloc_ptr; ++i) {
        //オブジェクトが再生位置にあったら
        if (obj->frame_begin < now_frame && now_frame < obj->frame_end) {

            for(int j = 0;j < select_len;++j){
                if (select[j] == obj->layer_disp) {
                    SelectingObjectIndex[n] = i;
                    n++;
                    break;
                }
            }
        }
        obj++;
        if ((int)obj->flag == 0) break; //flagが0だったら終端なので処理を終了する
        if (n >= select_len) break; //選択すべきものを全て選択したら処理を終了する
    }

    *SelectingObjectNum_ptr = n;    //選択した数を代入

    //選択したオブジェクトが一つも無ければ処理を終了する
    if (*SelectingObjectNum_ptr <= 0) return;

    int split_mode_org = *split_mode;   //org:originalの略
    *split_mode = 1;
    SendMessageA(exeditfp->hwnd, WM_COMMAND, 1051, -1); //"選択したオブジェクト"をカットする


    *split_mode = split_mode_org;

    *SelectingObjectNum_ptr = 0;
    drawtimeline(); //選択解除
}


//設定ファイルからデータを読み込み、select1～10配列に値を設定する
int set_customselect(HWND hwnd, FILTER* fp) {
    char buf[4096];
    char tmp[64];
    int cnt;
    int di_cnt;
    int j;
    int select_cnt;
    FILE* m_fp;

    //ファイルを読み取りモードで開く
    fopen_s(&m_fp, "m_supercut_config.ini", "r");

    //開けなければファイルを初期状態で作成する
    if (m_fp == NULL) {
        //ファイル作成
        fopen_s(&m_fp, "m_supercut_config.ini", "w");

        //一応書いてはいるがここに分岐することはないはず
        if (m_fp == NULL) {
            MessageBoxA(fp->hwnd, "set_customselect():m_supercut_config.iniファイル作成に失敗", fp->name, MB_OK);
            return -1;
        }

        //ファイルに初期データを書き込む
        fprintf(m_fp, "---------------------------------------------------------------------------------------------------\n");
        fprintf(m_fp, "注意:このファイルに無駄な文字や改行を入れないでください。入れるとプラグインが正常に動作しません。\n");
        fprintf(m_fp, "---------------------------------------------------------------------------------------------------\n\n");
        fprintf(m_fp, "custom1=1,2,3,4\n");
        fprintf(m_fp, "custom2=1,3,5,7\n");
        fprintf(m_fp, "custom3=1,3,4,5,7,8,9,10\n");
        fprintf(m_fp, "custom4=1,2,3,4,5,6,7,8,9,10,11,12,13,14,15\n");
        fprintf(m_fp, "custom5=1,10,20,30,40,50,60,70,80,90,100\n");
        fprintf(m_fp, "custom6=\n");
        fprintf(m_fp, "custom7=\n");
        fprintf(m_fp, "custom8=\n");
        fprintf(m_fp, "custom9=\n");
        fprintf(m_fp, "custom10=\n");

        fclose(m_fp);

        //ファイルを読み取りモードで開く
        fopen_s(&m_fp, "m_supercut_config.ini", "r");

        //一応書いてはいるがここに分岐することはないはず
        if (m_fp == NULL) {
            MessageBoxA(fp->hwnd, "set_customselect():m_supercut_config.iniファイル読み取りに失敗(ファイル作成は成功)", fp->name, MB_OK);
            return -1;
        }
    }

    //最初の注意のところを飛ばす(なければエラー)
    for (int i = 0; i < 4; ++i) {
        if (fgets(buf, sizeof(buf), m_fp) == NULL) {
            MessageBoxA(fp->hwnd, "(1～4行目)m_supercut_config.iniファイルの構文が間違っています", fp->name, MB_OK);
            return -2;
        }
    }

    //データ読み取り
    j = 0;
    cnt = 8;
    di_cnt = 0;
    select_cnt = 0;
    if (fgets(buf, sizeof(buf), m_fp) == NULL) {
        MessageBoxA(fp->hwnd, "(5行目)m_supercut_config.iniファイルの構文が間違っています", fp->name, MB_OK);
        return -3;
    }
    //文字列の終端までループ
    while (buf[cnt] != '\0') {
        if (buf[cnt] == ',') {
            tmp[j] = '\0';
            select1[select_cnt] = atoi(tmp) - 1;    //読み取った値を整数にして配列に代入(レイヤー番号とプログラム上で扱う値は1ずれてるので-1する)
            memset(tmp, '\0', sizeof(tmp));     //tmp配列の初期化
            select_cnt++;
            j = 0;
            di_cnt = 0;
            cnt++;
        }
        //4桁以上の数字が出てくることはありえないのでエラーを吐く
        else if (di_cnt >= 4) {
            sprintf_s(buf, sizeof(buf), "(m_supercut_config.ini5行目%d文字)エラー:4桁以上の数字が出てきています", cnt + 1);
            MessageBoxA(fp->hwnd, buf, fp->name, MB_OK);
            return -3;
        }
        else {
            tmp[j] = buf[cnt];
            j++;
            cnt++;
            di_cnt++;
        }
    }
    tmp[j] = '\0';
    select1[select_cnt] = atoi(tmp) - 1;    //読み取った値を整数にして配列に代入(レイヤー番号とプログラム上で扱う値は1ずれてるので-1する)
    memset(tmp, '\0', sizeof(tmp));     //tmp配列の初期化
    select_cnt++;
    j = 0;
    di_cnt = 0;
    cnt++;
    select1[select_cnt] = -1;



    j = 0;
    cnt = 8;
    di_cnt = 0;
    select_cnt = 0;
    if (fgets(buf, sizeof(buf), m_fp) == NULL) {
        MessageBoxA(fp->hwnd, "(6行目)m_supercut_config.iniファイルの構文が間違っています", fp->name, MB_OK);
        return -3;
    }
    //文字列の終端までループ
    while (buf[cnt] != '\0') {
        if (buf[cnt] == ',') {
            tmp[j] = '\0';
            select2[select_cnt] = atoi(tmp) - 1;    //読み取った値を整数にして配列に代入(レイヤー番号とプログラム上で扱う値は1ずれてるので-1する)
            memset(tmp, '\0', sizeof(tmp));     //tmp配列の初期化
            select_cnt++;
            j = 0;
            di_cnt = 0;
            cnt++;
        }
        //4桁以上の数字が出てくることはありえないのでエラーを吐く
        else if (di_cnt >= 4) {
            sprintf_s(buf, sizeof(buf), "(m_supercut_config.ini6行目%d文字)エラー:4桁以上の数字が出てきています", cnt + 1);
            MessageBoxA(fp->hwnd, buf, fp->name, MB_OK);
            return -3;
        }
        else {
            tmp[j] = buf[cnt];
            j++;
            cnt++;
            di_cnt++;
        }
    }
    tmp[j] = '\0';
    select2[select_cnt] = atoi(tmp) - 1;    //読み取った値を整数にして配列に代入(レイヤー番号とプログラム上で扱う値は1ずれてるので-1する)
    memset(tmp, '\0', sizeof(tmp));     //tmp配列の初期化
    select_cnt++;
    j = 0;
    di_cnt = 0;
    cnt++;
    select2[select_cnt] = -1;


    j = 0;
    cnt = 8;
    di_cnt = 0;
    select_cnt = 0;
    if (fgets(buf, sizeof(buf), m_fp) == NULL) {
        MessageBoxA(fp->hwnd, "(7行目)m_supercut_config.iniファイルの構文が間違っています", fp->name, MB_OK);
        return -3;
    }
    //文字列の終端までループ
    while (buf[cnt] != '\0') {
        if (buf[cnt] == ',') {
            tmp[j] = '\0';
            select3[select_cnt] = atoi(tmp) - 1;    //読み取った値を整数にして配列に代入(レイヤー番号とプログラム上で扱う値は1ずれてるので-1する)
            memset(tmp, '\0', sizeof(tmp));     //tmp配列の初期化
            select_cnt++;
            j = 0;
            di_cnt = 0;
            cnt++;
        }
        //4桁以上の数字が出てくることはありえないのでエラーを吐く
        else if (di_cnt >= 4) {
            sprintf_s(buf, sizeof(buf), "(m_supercut_config.ini7行目%d文字)エラー:4桁以上の数字が出てきています", cnt + 1);
            MessageBoxA(fp->hwnd, buf, fp->name, MB_OK);
            return -3;
        }
        else {
            tmp[j] = buf[cnt];
            j++;
            cnt++;
            di_cnt++;
        }
    }
    tmp[j] = '\0';
    select3[select_cnt] = atoi(tmp) - 1;    //読み取った値を整数にして配列に代入(レイヤー番号とプログラム上で扱う値は1ずれてるので-1する)
    memset(tmp, '\0', sizeof(tmp));     //tmp配列の初期化
    select_cnt++;
    j = 0;
    di_cnt = 0;
    cnt++;
    select3[select_cnt] = -1;



    j = 0;
    cnt = 8;
    di_cnt = 0;
    select_cnt = 0;
    if (fgets(buf, sizeof(buf), m_fp) == NULL) {
        MessageBoxA(fp->hwnd, "(8行目)m_supercut_config.iniファイルの構文が間違っています", fp->name, MB_OK);
        return -3;
    }
    //文字列の終端までループ
    while (buf[cnt] != '\0') {
        if (buf[cnt] == ',') {
            tmp[j] = '\0';
            select4[select_cnt] = atoi(tmp) - 1;    //読み取った値を整数にして配列に代入(レイヤー番号とプログラム上で扱う値は1ずれてるので-1する)
            memset(tmp, '\0', sizeof(tmp));     //tmp配列の初期化
            select_cnt++;
            j = 0;
            di_cnt = 0;
            cnt++;
        }
        //4桁以上の数字が出てくることはありえないのでエラーを吐く
        else if (di_cnt >= 4) {
            sprintf_s(buf, sizeof(buf), "(m_supercut_config.ini8行目%d文字)エラー:4桁以上の数字が出てきています", cnt + 1);
            MessageBoxA(fp->hwnd, buf, fp->name, MB_OK);
            return -3;
        }
        else {
            tmp[j] = buf[cnt];
            j++;
            cnt++;
            di_cnt++;
        }
    }
    tmp[j] = '\0';
    select4[select_cnt] = atoi(tmp) - 1;    //読み取った値を整数にして配列に代入(レイヤー番号とプログラム上で扱う値は1ずれてるので-1する)
    memset(tmp, '\0', sizeof(tmp));     //tmp配列の初期化
    select_cnt++;
    j = 0;
    di_cnt = 0;
    cnt++;
    select4[select_cnt] = -1;




    j = 0;
    cnt = 8;
    di_cnt = 0;
    select_cnt = 0;
    if (fgets(buf, sizeof(buf), m_fp) == NULL) {
        MessageBoxA(fp->hwnd, "(9行目)m_supercut_config.iniファイルの構文が間違っています", fp->name, MB_OK);
        return -3;
    }
    //文字列の終端までループ
    while (buf[cnt] != '\0') {
        if (buf[cnt] == ',') {
            tmp[j] = '\0';
            select5[select_cnt] = atoi(tmp) - 1;    //読み取った値を整数にして配列に代入(レイヤー番号とプログラム上で扱う値は1ずれてるので-1する)
            memset(tmp, '\0', sizeof(tmp));     //tmp配列の初期化
            select_cnt++;
            j = 0;
            di_cnt = 0;
            cnt++;
        }
        //4桁以上の数字が出てくることはありえないのでエラーを吐く
        else if (di_cnt >= 4) {
            sprintf_s(buf, sizeof(buf), "(m_supercut_config.ini9行目%d文字)エラー:4桁以上の数字が出てきています", cnt + 1);
            MessageBoxA(fp->hwnd, buf, fp->name, MB_OK);
            return -3;
        }
        else {
            tmp[j] = buf[cnt];
            j++;
            cnt++;
            di_cnt++;
        }
    }
    tmp[j] = '\0';
    select5[select_cnt] = atoi(tmp) - 1;    //読み取った値を整数にして配列に代入(レイヤー番号とプログラム上で扱う値は1ずれてるので-1する)
    memset(tmp, '\0', sizeof(tmp));     //tmp配列の初期化
    select_cnt++;
    j = 0;
    di_cnt = 0;
    cnt++;
    select5[select_cnt] = -1;



    j = 0;
    cnt = 8;
    di_cnt = 0;
    select_cnt = 0;
    if (fgets(buf, sizeof(buf), m_fp) == NULL) {
        MessageBoxA(fp->hwnd, "(10行目)m_supercut_config.iniファイルの構文が間違っています", fp->name, MB_OK);
        return -3;
    }
    //文字列の終端までループ
    while (buf[cnt] != '\0') {
        if (buf[cnt] == ',') {
            tmp[j] = '\0';
            select6[select_cnt] = atoi(tmp) - 1;    //読み取った値を整数にして配列に代入(レイヤー番号とプログラム上で扱う値は1ずれてるので-1する)
            memset(tmp, '\0', sizeof(tmp));     //tmp配列の初期化
            select_cnt++;
            j = 0;
            di_cnt = 0;
            cnt++;
        }
        //4桁以上の数字が出てくることはありえないのでエラーを吐く
        else if (di_cnt >= 4) {
            sprintf_s(buf, sizeof(buf), "(m_supercut_config.ini10行目%d文字)エラー:4桁以上の数字が出てきています", cnt + 1);
            MessageBoxA(fp->hwnd, buf, fp->name, MB_OK);
            return -3;
        }
        else {
            tmp[j] = buf[cnt];
            j++;
            cnt++;
            di_cnt++;
        }
    }
    tmp[j] = '\0';
    select6[select_cnt] = atoi(tmp) - 1;    //読み取った値を整数にして配列に代入(レイヤー番号とプログラム上で扱う値は1ずれてるので-1する)
    memset(tmp, '\0', sizeof(tmp));     //tmp配列の初期化
    select_cnt++;
    j = 0;
    di_cnt = 0;
    cnt++;
    select6[select_cnt] = -1;


    j = 0;
    cnt = 8;
    di_cnt = 0;
    select_cnt = 0;
    if (fgets(buf, sizeof(buf), m_fp) == NULL) {
        MessageBoxA(fp->hwnd, "(11行目)m_supercut_config.iniファイルの構文が間違っています", fp->name, MB_OK);
        return -3;
    }
    //文字列の終端までループ
    while (buf[cnt] != '\0') {
        if (buf[cnt] == ',') {
            tmp[j] = '\0';
            select7[select_cnt] = atoi(tmp) - 1;    //読み取った値を整数にして配列に代入(レイヤー番号とプログラム上で扱う値は1ずれてるので-1する)
            memset(tmp, '\0', sizeof(tmp));     //tmp配列の初期化
            select_cnt++;
            j = 0;
            di_cnt = 0;
            cnt++;
        }
        //4桁以上の数字が出てくることはありえないのでエラーを吐く
        else if (di_cnt >= 4) {
            sprintf_s(buf, sizeof(buf), "(m_supercut_config.ini11行目%d文字)エラー:4桁以上の数字が出てきています", cnt + 1);
            MessageBoxA(fp->hwnd, buf, fp->name, MB_OK);
            return -3;
        }
        else {
            tmp[j] = buf[cnt];
            j++;
            cnt++;
            di_cnt++;
        }
    }
    tmp[j] = '\0';
    select7[select_cnt] = atoi(tmp) - 1;    //読み取った値を整数にして配列に代入(レイヤー番号とプログラム上で扱う値は1ずれてるので-1する)
    memset(tmp, '\0', sizeof(tmp));     //tmp配列の初期化
    select_cnt++;
    j = 0;
    di_cnt = 0;
    cnt++;
    select7[select_cnt] = -1;



    j = 0;
    cnt = 8;
    di_cnt = 0;
    select_cnt = 0;
    if (fgets(buf, sizeof(buf), m_fp) == NULL) {
        MessageBoxA(fp->hwnd, "(12行目)m_supercut_config.iniファイルの構文が間違っています", fp->name, MB_OK);
        return -3;
    }
    //文字列の終端までループ
    while (buf[cnt] != '\0') {
        if (buf[cnt] == ',') {
            tmp[j] = '\0';
            select8[select_cnt] = atoi(tmp) - 1;    //読み取った値を整数にして配列に代入(レイヤー番号とプログラム上で扱う値は1ずれてるので-1する)
            memset(tmp, '\0', sizeof(tmp));     //tmp配列の初期化
            select_cnt++;
            j = 0;
            di_cnt = 0;
            cnt++;
        }
        //4桁以上の数字が出てくることはありえないのでエラーを吐く
        else if (di_cnt >= 4) {
            sprintf_s(buf, sizeof(buf), "(m_supercut_config.ini12行目%d文字)エラー:4桁以上の数字が出てきています", cnt + 1);
            MessageBoxA(fp->hwnd, buf, fp->name, MB_OK);
            return -3;
        }
        else {
            tmp[j] = buf[cnt];
            j++;
            cnt++;
            di_cnt++;
        }
    }
    tmp[j] = '\0';
    select8[select_cnt] = atoi(tmp) - 1;    //読み取った値を整数にして配列に代入(レイヤー番号とプログラム上で扱う値は1ずれてるので-1する)
    memset(tmp, '\0', sizeof(tmp));     //tmp配列の初期化
    select_cnt++;
    j = 0;
    di_cnt = 0;
    cnt++;
    select8[select_cnt] = -1;




    j = 0;
    cnt = 8;
    di_cnt = 0;
    select_cnt = 0;
    if (fgets(buf, sizeof(buf), m_fp) == NULL) {
        MessageBoxA(fp->hwnd, "(13行目)m_supercut_config.iniファイルの構文が間違っています", fp->name, MB_OK);
        return -3;
    }
    //文字列の終端までループ
    while (buf[cnt] != '\0') {
        if (buf[cnt] == ',') {
            tmp[j] = '\0';
            select9[select_cnt] = atoi(tmp) - 1;    //読み取った値を整数にして配列に代入(レイヤー番号とプログラム上で扱う値は1ずれてるので-1する)
            memset(tmp, '\0', sizeof(tmp));     //tmp配列の初期化
            select_cnt++;
            j = 0;
            di_cnt = 0;
            cnt++;
        }
        //4桁以上の数字が出てくることはありえないのでエラーを吐く
        else if (di_cnt >= 4) {
            sprintf_s(buf, sizeof(buf), "(m_supercut_config.ini13行目%d文字)エラー:4桁以上の数字が出てきています", cnt + 1);
            MessageBoxA(fp->hwnd, buf, fp->name, MB_OK);
            return -3;
        }
        else {
            tmp[j] = buf[cnt];
            j++;
            cnt++;
            di_cnt++;
        }
    }
    tmp[j] = '\0';
    select9[select_cnt] = atoi(tmp) - 1;    //読み取った値を整数にして配列に代入(レイヤー番号とプログラム上で扱う値は1ずれてるので-1する)
    memset(tmp, '\0', sizeof(tmp));     //tmp配列の初期化
    select_cnt++;
    j = 0;
    di_cnt = 0;
    cnt++;
    select9[select_cnt] = -1;



    j = 0;
    cnt = 9;
    di_cnt = 0;
    select_cnt = 0;
    if (fgets(buf, sizeof(buf), m_fp) == NULL) {
        MessageBoxA(fp->hwnd, "(14行目)m_supercut_config.iniファイルの構文が間違っています", fp->name, MB_OK);
        return -3;
    }
    //文字列の終端までループ
    while (buf[cnt] != '\0') {
        if (buf[cnt] == ',') {
            tmp[j] = '\0';
            select10[select_cnt] = atoi(tmp) - 1;    //読み取った値を整数にして配列に代入(レイヤー番号とプログラム上で扱う値は1ずれてるので-1する)
            memset(tmp, '\0', sizeof(tmp));     //tmp配列の初期化
            select_cnt++;
            j = 0;
            di_cnt = 0;
            cnt++;
        }
        //4桁以上の数字が出てくることはありえないのでエラーを吐く
        else if (di_cnt >= 4) {
            sprintf_s(buf, sizeof(buf), "(m_supercut_config.ini14行目%d文字)エラー:4桁以上の数字が出てきています", cnt + 1);
            MessageBoxA(fp->hwnd, buf, fp->name, MB_OK);
            return -3;
        }
        else {
            tmp[j] = buf[cnt];
            j++;
            cnt++;
            di_cnt++;
        }
    }
    tmp[j] = '\0';
    select10[select_cnt] = atoi(tmp) - 1;    //読み取った値を整数にして配列に代入(レイヤー番号とプログラム上で扱う値は1ずれてるので-1する)
    memset(tmp, '\0', sizeof(tmp));     //tmp配列の初期化
    select_cnt++;
    j = 0;
    di_cnt = 0;
    cnt++;
    select10[select_cnt] = -1;

    fclose(m_fp);

    return 0;
}




BOOL func_WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, void* editp, FILTER* fp) {
    switch (message) {

    //(多分)最初に実行される部分
    case WM_FILTER_INIT:
        //データの読み取り
        set_customselect(hwnd, fp);

        //拡張編集0.92があるかの判別
        exeditfp = get_exeditfp(fp);
        if (exeditfp == NULL) {
            MessageBoxA(fp->hwnd, "拡張編集0.92が見つかりませんでした", fp->name, MB_OK);
            break;
        }

        //ショートカットキーの追加
        fp->exfunc->add_menu_item(fp, (LPSTR)all_split, fp->hwnd, M_ITEM_ALL_SPLIT, NULL, NULL);
        fp->exfunc->add_menu_item(fp, (LPSTR)custom1_split, fp->hwnd, M_ITEM_CUSTOM1_SPLIT, NULL, NULL);
        fp->exfunc->add_menu_item(fp, (LPSTR)custom2_split, fp->hwnd, M_ITEM_CUSTOM2_SPLIT, NULL, NULL);
        fp->exfunc->add_menu_item(fp, (LPSTR)custom3_split, fp->hwnd, M_ITEM_CUSTOM3_SPLIT, NULL, NULL);
        fp->exfunc->add_menu_item(fp, (LPSTR)custom4_split, fp->hwnd, M_ITEM_CUSTOM4_SPLIT, NULL, NULL);
        fp->exfunc->add_menu_item(fp, (LPSTR)custom5_split, fp->hwnd, M_ITEM_CUSTOM5_SPLIT, NULL, NULL);
        fp->exfunc->add_menu_item(fp, (LPSTR)custom6_split, fp->hwnd, M_ITEM_CUSTOM6_SPLIT, NULL, NULL);
        fp->exfunc->add_menu_item(fp, (LPSTR)custom7_split, fp->hwnd, M_ITEM_CUSTOM7_SPLIT, NULL, NULL);
        fp->exfunc->add_menu_item(fp, (LPSTR)custom8_split, fp->hwnd, M_ITEM_CUSTOM8_SPLIT, NULL, NULL);
        fp->exfunc->add_menu_item(fp, (LPSTR)custom9_split, fp->hwnd, M_ITEM_CUSTOM9_SPLIT, NULL, NULL);
        fp->exfunc->add_menu_item(fp, (LPSTR)custom10_split, fp->hwnd, M_ITEM_CUSTOM10_SPLIT, NULL, NULL);

        
        ObjectAlloc_ptr = (int*)((int)exeditfp->dll_hinst + 0x1e0fa0);
        ObjectArray_ptr = (ExEdit::Object**)((int)exeditfp->dll_hinst + 0x1e0fa4);
        SelectingObjectNum_ptr = (int*)((int)exeditfp->dll_hinst + 0x167d88);
        SelectingObjectIndex = (int*)((int)exeditfp->dll_hinst + 0x179230);
        split_mode = (int*)((int)exeditfp->dll_hinst + 0x1538b0);
        drawtimeline = reinterpret_cast<decltype(drawtimeline)>((int)exeditfp->dll_hinst + 0x39230);


        break;
    case WM_FILTER_COMMAND:
        switch (wparam) {
        case M_ITEM_ALL_SPLIT:
            item_all_split(editp, fp);
            break;
        case M_ITEM_CUSTOM1_SPLIT:
            item_custom_split(editp, fp, select1);
            break;
        case M_ITEM_CUSTOM2_SPLIT:
            item_custom_split(editp, fp, select2);
            break;
        case M_ITEM_CUSTOM3_SPLIT:
            item_custom_split(editp, fp, select3);
            break;
        case M_ITEM_CUSTOM4_SPLIT:
            item_custom_split(editp, fp, select4);
            break;
        case M_ITEM_CUSTOM5_SPLIT:
            item_custom_split(editp, fp, select5);
            break;
        case M_ITEM_CUSTOM6_SPLIT:
            item_custom_split(editp, fp, select6);
            break;
        case M_ITEM_CUSTOM7_SPLIT:
            item_custom_split(editp, fp, select7);
            break;
        case M_ITEM_CUSTOM8_SPLIT:
            item_custom_split(editp, fp, select8);
            break;
        case M_ITEM_CUSTOM9_SPLIT:
            item_custom_split(editp, fp, select9);
            break;
        case M_ITEM_CUSTOM10_SPLIT:
            item_custom_split(editp, fp, select10);
            break;
        }
        break;
    case WM_COMMAND:
        switch (wparam) {
        struct _stat s;
        case MID_FILTER_BUTTON:
            //説明ファイルが無ければ新規作成
            if (_stat("m_supercut_config_inst.txt", &s) != 0) {
                FILE* m_fp;

                //ファイル作成
                fopen_s(&m_fp, "m_supercut_config_inst.txt", "w");
                if (m_fp == NULL) break;

                //ファイルに説明を書き込む
                fprintf(m_fp, "注意:configファイルに無駄な文字や改行を入れないでください。入れるとプラグインが正常に動作しません。\n\n");
                fprintf(m_fp, "- スーパーカット,分割カスタム1～10の設定方法 -\n");
                fprintf(m_fp, "configファイルの\" = \"の後に、分割したいレイヤー番号を全て コンマ区切り で書きます\n");
                fprintf(m_fp, "レイヤー番号は昇順(小さい数から大きい数)になるように書いてください\n\n\n");
                fprintf(m_fp, "例えば、custom1を以下のように書き、aviutlメニューバーの編集→スーパーカット→分割カスタム1をクリックすると(もしくは対応するショートカットキーを押すと)\n");
                fprintf(m_fp, "custom1=1,2,3\n");
                fprintf(m_fp, "レイヤー1,2,3にあるオブジェクトが再生位置で分割されます\n\n");
                fprintf(m_fp, "設定できるのはcustom1～custom10までです、custom11= と書いても分割カスタム11が使えるといったことはありません。(今後対応する可能性はありますが...)\n");

                fclose(m_fp);
            }
            //ファイルを開く
            system("m_supercut_config_inst.txt");
            
            break;
        case MID_FILTER_BUTTON+1:
            //設定ファイルがなければ新規作成
            if (_stat("m_supercut_config.ini", &s) != 0) {
                FILE* m_fp;

                //ファイル作成
                fopen_s(&m_fp, "m_supercut_config.ini", "w");
                if (m_fp == NULL) break;

                //ファイルに初期データを書き込む
                fprintf(m_fp, "---------------------------------------------------------------------------------------------------\n");
                fprintf(m_fp, "注意:このファイルに無駄な文字や改行を入れないでください。入れるとプラグインが正常に動作しません。\n");
                fprintf(m_fp, "---------------------------------------------------------------------------------------------------\n\n");
                fprintf(m_fp, "custom1=1,2,3,4\n");
                fprintf(m_fp, "custom2=1,3,5,7\n");
                fprintf(m_fp, "custom3=1,3,4,5,7,8,9,10\n");
                fprintf(m_fp, "custom4=1,2,3,4,5,6,7,8,9,10,11,12,13,14,15\n");
                fprintf(m_fp, "custom5=1,10,20,30,40,50,60,70,80,90,100\n");
                fprintf(m_fp, "custom6=\n");
                fprintf(m_fp, "custom7=\n");
                fprintf(m_fp, "custom8=\n");
                fprintf(m_fp, "custom9=\n");
                fprintf(m_fp, "custom10=\n");

                fclose(m_fp);
            }

            //設定ファイルを開く
            system("m_supercut_config.ini");

            //設定ファイルの情報を反映
            set_customselect(hwnd, fp);


            break;
        }
        break;
    }
    return FALSE;
}