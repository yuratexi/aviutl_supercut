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

static const char all_split[] = ("����ALL");
static const char custom1_split[] = ("�����J�X�^��1");
static const char custom2_split[] = ("�����J�X�^��2");
static const char custom3_split[] = ("�����J�X�^��3");
static const char custom4_split[] = ("�����J�X�^��4");
static const char custom5_split[] = ("�����J�X�^��5");
static const char custom6_split[] = ("�����J�X�^��6");
static const char custom7_split[] = ("�����J�X�^��7");
static const char custom8_split[] = ("�����J�X�^��8");
static const char custom9_split[] = ("�����J�X�^��9");
static const char custom10_split[] = ("�����J�X�^��10");


//constexpr TCHAR* track_name[] = { "1","2","3","4","5" };
//constexpr int    track_default[] = { 0,0,0,0,0 };

constexpr TCHAR* track_name[] = { "config�t�@�C���̐ݒ���@���m�F����","config�t�@�C�����J��"};
constexpr int    track_default[] = { -1,-1 };


FILTER_DLL filter = {
    FILTER_FLAG_ALWAYS_ACTIVE,
    NULL,NULL,
    const_cast<char*>("�X�[�p�[�J�b�g by ���Ă��["),
    NULL,NULL,NULL,
    NULL,NULL,
    2,          //�`�F�b�N�{�b�N�X�̐�
    const_cast<TCHAR**>(track_name),    //�`�F�b�N�{�b�N�X���O�S�ւ̃|�C���^(���O�������Ă�z�񖼂�����)
    const_cast<int*>(track_default),    //�����l�ւ̃|�C���^
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
            if (!strcmp(tfp->information, "�g���ҏW(exedit) version 0.92 by �j�d�m����")) return tfp;
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
    int now_frame = fp->exfunc->get_frame(editp);    //���݂̃t���[���ʒu
    auto obj = *ObjectArray_ptr;

    //�Đ��ʒu�ɑ��݂���I�u�W�F�N�g��S�đI��
    for (int i = 0; i < *ObjectAlloc_ptr; ++i) {
        //�I�u�W�F�N�g���Đ��ʒu�ɂ�������
        if (obj->frame_begin <= now_frame && now_frame <= obj->frame_end) {
            SelectingObjectIndex[n] = i;
            n++;
        }
        obj++;
        if ((int)obj->flag == 0) break; //flag��0��������I�[�Ȃ̂ŏ������I������
    }

    *SelectingObjectNum_ptr = n;    //�I������������
    
    int split_mode_org = *split_mode;   //org:original�̗�
    *split_mode = 1;
    SendMessageA(exeditfp->hwnd, WM_COMMAND, 1051, -1); //"�I�������I�u�W�F�N�g"���J�b�g����


    *split_mode = split_mode_org;

    *SelectingObjectNum_ptr = 0;
    drawtimeline(); //�I������
}

//select�̓Y����101�Ȃ̂́A���C���[�̐�(100)+�I�[�����p(1)������
void item_custom_split(void* editp, FILTER* fp,int select[101]) {
    int select_len = 0; //�z��select�̒���

    bool findflg = false;
    int n = 0;
    int now_frame = fp->exfunc->get_frame(editp);    //���݂̃t���[���ʒu

    auto obj = *ObjectArray_ptr;

    
    //select_len�����߂�
    while (select[select_len] != -1) {
        select_len++;
    }

    //*ObjectAlloc_ptr=�f�[�^�̌�(���)
    for (int i = 0; i < *ObjectAlloc_ptr; ++i) {
        //�I�u�W�F�N�g���Đ��ʒu�ɂ�������
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
        if ((int)obj->flag == 0) break; //flag��0��������I�[�Ȃ̂ŏ������I������
        if (n >= select_len) break; //�I�����ׂ����̂�S�đI�������珈�����I������
    }

    *SelectingObjectNum_ptr = n;    //�I������������

    //�I�������I�u�W�F�N�g�����������Ώ������I������
    if (*SelectingObjectNum_ptr <= 0) return;

    int split_mode_org = *split_mode;   //org:original�̗�
    *split_mode = 1;
    SendMessageA(exeditfp->hwnd, WM_COMMAND, 1051, -1); //"�I�������I�u�W�F�N�g"���J�b�g����


    *split_mode = split_mode_org;

    *SelectingObjectNum_ptr = 0;
    drawtimeline(); //�I������
}


//�ݒ�t�@�C������f�[�^��ǂݍ��݁Aselect1�`10�z��ɒl��ݒ肷��
int set_customselect(HWND hwnd, FILTER* fp) {
    char buf[4096];
    char tmp[64];
    int cnt;
    int di_cnt;
    int j;
    int select_cnt;
    FILE* m_fp;

    //�t�@�C����ǂݎ�胂�[�h�ŊJ��
    fopen_s(&m_fp, "m_supercut_config.ini", "r");

    //�J���Ȃ���΃t�@�C����������Ԃō쐬����
    if (m_fp == NULL) {
        //�t�@�C���쐬
        fopen_s(&m_fp, "m_supercut_config.ini", "w");

        //�ꉞ�����Ă͂��邪�����ɕ��򂷂邱�Ƃ͂Ȃ��͂�
        if (m_fp == NULL) {
            MessageBoxA(fp->hwnd, "set_customselect():m_supercut_config.ini�t�@�C���쐬�Ɏ��s", fp->name, MB_OK);
            return -1;
        }

        //�t�@�C���ɏ����f�[�^����������
        fprintf(m_fp, "---------------------------------------------------------------------------------------------------\n");
        fprintf(m_fp, "����:���̃t�@�C���ɖ��ʂȕ�������s�����Ȃ��ł��������B�����ƃv���O�C��������ɓ��삵�܂���B\n");
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

        //�t�@�C����ǂݎ�胂�[�h�ŊJ��
        fopen_s(&m_fp, "m_supercut_config.ini", "r");

        //�ꉞ�����Ă͂��邪�����ɕ��򂷂邱�Ƃ͂Ȃ��͂�
        if (m_fp == NULL) {
            MessageBoxA(fp->hwnd, "set_customselect():m_supercut_config.ini�t�@�C���ǂݎ��Ɏ��s(�t�@�C���쐬�͐���)", fp->name, MB_OK);
            return -1;
        }
    }

    //�ŏ��̒��ӂ̂Ƃ�����΂�(�Ȃ���΃G���[)
    for (int i = 0; i < 4; ++i) {
        if (fgets(buf, sizeof(buf), m_fp) == NULL) {
            MessageBoxA(fp->hwnd, "(1�`4�s��)m_supercut_config.ini�t�@�C���̍\�����Ԉ���Ă��܂�", fp->name, MB_OK);
            return -2;
        }
    }

    //�f�[�^�ǂݎ��
    j = 0;
    cnt = 8;
    di_cnt = 0;
    select_cnt = 0;
    if (fgets(buf, sizeof(buf), m_fp) == NULL) {
        MessageBoxA(fp->hwnd, "(5�s��)m_supercut_config.ini�t�@�C���̍\�����Ԉ���Ă��܂�", fp->name, MB_OK);
        return -3;
    }
    //������̏I�[�܂Ń��[�v
    while (buf[cnt] != '\0') {
        if (buf[cnt] == ',') {
            tmp[j] = '\0';
            select1[select_cnt] = atoi(tmp) - 1;    //�ǂݎ�����l�𐮐��ɂ��Ĕz��ɑ��(���C���[�ԍ��ƃv���O������ň����l��1����Ă�̂�-1����)
            memset(tmp, '\0', sizeof(tmp));     //tmp�z��̏�����
            select_cnt++;
            j = 0;
            di_cnt = 0;
            cnt++;
        }
        //4���ȏ�̐������o�Ă��邱�Ƃ͂��肦�Ȃ��̂ŃG���[��f��
        else if (di_cnt >= 4) {
            sprintf_s(buf, sizeof(buf), "(m_supercut_config.ini5�s��%d����)�G���[:4���ȏ�̐������o�Ă��Ă��܂�", cnt + 1);
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
    select1[select_cnt] = atoi(tmp) - 1;    //�ǂݎ�����l�𐮐��ɂ��Ĕz��ɑ��(���C���[�ԍ��ƃv���O������ň����l��1����Ă�̂�-1����)
    memset(tmp, '\0', sizeof(tmp));     //tmp�z��̏�����
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
        MessageBoxA(fp->hwnd, "(6�s��)m_supercut_config.ini�t�@�C���̍\�����Ԉ���Ă��܂�", fp->name, MB_OK);
        return -3;
    }
    //������̏I�[�܂Ń��[�v
    while (buf[cnt] != '\0') {
        if (buf[cnt] == ',') {
            tmp[j] = '\0';
            select2[select_cnt] = atoi(tmp) - 1;    //�ǂݎ�����l�𐮐��ɂ��Ĕz��ɑ��(���C���[�ԍ��ƃv���O������ň����l��1����Ă�̂�-1����)
            memset(tmp, '\0', sizeof(tmp));     //tmp�z��̏�����
            select_cnt++;
            j = 0;
            di_cnt = 0;
            cnt++;
        }
        //4���ȏ�̐������o�Ă��邱�Ƃ͂��肦�Ȃ��̂ŃG���[��f��
        else if (di_cnt >= 4) {
            sprintf_s(buf, sizeof(buf), "(m_supercut_config.ini6�s��%d����)�G���[:4���ȏ�̐������o�Ă��Ă��܂�", cnt + 1);
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
    select2[select_cnt] = atoi(tmp) - 1;    //�ǂݎ�����l�𐮐��ɂ��Ĕz��ɑ��(���C���[�ԍ��ƃv���O������ň����l��1����Ă�̂�-1����)
    memset(tmp, '\0', sizeof(tmp));     //tmp�z��̏�����
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
        MessageBoxA(fp->hwnd, "(7�s��)m_supercut_config.ini�t�@�C���̍\�����Ԉ���Ă��܂�", fp->name, MB_OK);
        return -3;
    }
    //������̏I�[�܂Ń��[�v
    while (buf[cnt] != '\0') {
        if (buf[cnt] == ',') {
            tmp[j] = '\0';
            select3[select_cnt] = atoi(tmp) - 1;    //�ǂݎ�����l�𐮐��ɂ��Ĕz��ɑ��(���C���[�ԍ��ƃv���O������ň����l��1����Ă�̂�-1����)
            memset(tmp, '\0', sizeof(tmp));     //tmp�z��̏�����
            select_cnt++;
            j = 0;
            di_cnt = 0;
            cnt++;
        }
        //4���ȏ�̐������o�Ă��邱�Ƃ͂��肦�Ȃ��̂ŃG���[��f��
        else if (di_cnt >= 4) {
            sprintf_s(buf, sizeof(buf), "(m_supercut_config.ini7�s��%d����)�G���[:4���ȏ�̐������o�Ă��Ă��܂�", cnt + 1);
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
    select3[select_cnt] = atoi(tmp) - 1;    //�ǂݎ�����l�𐮐��ɂ��Ĕz��ɑ��(���C���[�ԍ��ƃv���O������ň����l��1����Ă�̂�-1����)
    memset(tmp, '\0', sizeof(tmp));     //tmp�z��̏�����
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
        MessageBoxA(fp->hwnd, "(8�s��)m_supercut_config.ini�t�@�C���̍\�����Ԉ���Ă��܂�", fp->name, MB_OK);
        return -3;
    }
    //������̏I�[�܂Ń��[�v
    while (buf[cnt] != '\0') {
        if (buf[cnt] == ',') {
            tmp[j] = '\0';
            select4[select_cnt] = atoi(tmp) - 1;    //�ǂݎ�����l�𐮐��ɂ��Ĕz��ɑ��(���C���[�ԍ��ƃv���O������ň����l��1����Ă�̂�-1����)
            memset(tmp, '\0', sizeof(tmp));     //tmp�z��̏�����
            select_cnt++;
            j = 0;
            di_cnt = 0;
            cnt++;
        }
        //4���ȏ�̐������o�Ă��邱�Ƃ͂��肦�Ȃ��̂ŃG���[��f��
        else if (di_cnt >= 4) {
            sprintf_s(buf, sizeof(buf), "(m_supercut_config.ini8�s��%d����)�G���[:4���ȏ�̐������o�Ă��Ă��܂�", cnt + 1);
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
    select4[select_cnt] = atoi(tmp) - 1;    //�ǂݎ�����l�𐮐��ɂ��Ĕz��ɑ��(���C���[�ԍ��ƃv���O������ň����l��1����Ă�̂�-1����)
    memset(tmp, '\0', sizeof(tmp));     //tmp�z��̏�����
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
        MessageBoxA(fp->hwnd, "(9�s��)m_supercut_config.ini�t�@�C���̍\�����Ԉ���Ă��܂�", fp->name, MB_OK);
        return -3;
    }
    //������̏I�[�܂Ń��[�v
    while (buf[cnt] != '\0') {
        if (buf[cnt] == ',') {
            tmp[j] = '\0';
            select5[select_cnt] = atoi(tmp) - 1;    //�ǂݎ�����l�𐮐��ɂ��Ĕz��ɑ��(���C���[�ԍ��ƃv���O������ň����l��1����Ă�̂�-1����)
            memset(tmp, '\0', sizeof(tmp));     //tmp�z��̏�����
            select_cnt++;
            j = 0;
            di_cnt = 0;
            cnt++;
        }
        //4���ȏ�̐������o�Ă��邱�Ƃ͂��肦�Ȃ��̂ŃG���[��f��
        else if (di_cnt >= 4) {
            sprintf_s(buf, sizeof(buf), "(m_supercut_config.ini9�s��%d����)�G���[:4���ȏ�̐������o�Ă��Ă��܂�", cnt + 1);
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
    select5[select_cnt] = atoi(tmp) - 1;    //�ǂݎ�����l�𐮐��ɂ��Ĕz��ɑ��(���C���[�ԍ��ƃv���O������ň����l��1����Ă�̂�-1����)
    memset(tmp, '\0', sizeof(tmp));     //tmp�z��̏�����
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
        MessageBoxA(fp->hwnd, "(10�s��)m_supercut_config.ini�t�@�C���̍\�����Ԉ���Ă��܂�", fp->name, MB_OK);
        return -3;
    }
    //������̏I�[�܂Ń��[�v
    while (buf[cnt] != '\0') {
        if (buf[cnt] == ',') {
            tmp[j] = '\0';
            select6[select_cnt] = atoi(tmp) - 1;    //�ǂݎ�����l�𐮐��ɂ��Ĕz��ɑ��(���C���[�ԍ��ƃv���O������ň����l��1����Ă�̂�-1����)
            memset(tmp, '\0', sizeof(tmp));     //tmp�z��̏�����
            select_cnt++;
            j = 0;
            di_cnt = 0;
            cnt++;
        }
        //4���ȏ�̐������o�Ă��邱�Ƃ͂��肦�Ȃ��̂ŃG���[��f��
        else if (di_cnt >= 4) {
            sprintf_s(buf, sizeof(buf), "(m_supercut_config.ini10�s��%d����)�G���[:4���ȏ�̐������o�Ă��Ă��܂�", cnt + 1);
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
    select6[select_cnt] = atoi(tmp) - 1;    //�ǂݎ�����l�𐮐��ɂ��Ĕz��ɑ��(���C���[�ԍ��ƃv���O������ň����l��1����Ă�̂�-1����)
    memset(tmp, '\0', sizeof(tmp));     //tmp�z��̏�����
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
        MessageBoxA(fp->hwnd, "(11�s��)m_supercut_config.ini�t�@�C���̍\�����Ԉ���Ă��܂�", fp->name, MB_OK);
        return -3;
    }
    //������̏I�[�܂Ń��[�v
    while (buf[cnt] != '\0') {
        if (buf[cnt] == ',') {
            tmp[j] = '\0';
            select7[select_cnt] = atoi(tmp) - 1;    //�ǂݎ�����l�𐮐��ɂ��Ĕz��ɑ��(���C���[�ԍ��ƃv���O������ň����l��1����Ă�̂�-1����)
            memset(tmp, '\0', sizeof(tmp));     //tmp�z��̏�����
            select_cnt++;
            j = 0;
            di_cnt = 0;
            cnt++;
        }
        //4���ȏ�̐������o�Ă��邱�Ƃ͂��肦�Ȃ��̂ŃG���[��f��
        else if (di_cnt >= 4) {
            sprintf_s(buf, sizeof(buf), "(m_supercut_config.ini11�s��%d����)�G���[:4���ȏ�̐������o�Ă��Ă��܂�", cnt + 1);
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
    select7[select_cnt] = atoi(tmp) - 1;    //�ǂݎ�����l�𐮐��ɂ��Ĕz��ɑ��(���C���[�ԍ��ƃv���O������ň����l��1����Ă�̂�-1����)
    memset(tmp, '\0', sizeof(tmp));     //tmp�z��̏�����
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
        MessageBoxA(fp->hwnd, "(12�s��)m_supercut_config.ini�t�@�C���̍\�����Ԉ���Ă��܂�", fp->name, MB_OK);
        return -3;
    }
    //������̏I�[�܂Ń��[�v
    while (buf[cnt] != '\0') {
        if (buf[cnt] == ',') {
            tmp[j] = '\0';
            select8[select_cnt] = atoi(tmp) - 1;    //�ǂݎ�����l�𐮐��ɂ��Ĕz��ɑ��(���C���[�ԍ��ƃv���O������ň����l��1����Ă�̂�-1����)
            memset(tmp, '\0', sizeof(tmp));     //tmp�z��̏�����
            select_cnt++;
            j = 0;
            di_cnt = 0;
            cnt++;
        }
        //4���ȏ�̐������o�Ă��邱�Ƃ͂��肦�Ȃ��̂ŃG���[��f��
        else if (di_cnt >= 4) {
            sprintf_s(buf, sizeof(buf), "(m_supercut_config.ini12�s��%d����)�G���[:4���ȏ�̐������o�Ă��Ă��܂�", cnt + 1);
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
    select8[select_cnt] = atoi(tmp) - 1;    //�ǂݎ�����l�𐮐��ɂ��Ĕz��ɑ��(���C���[�ԍ��ƃv���O������ň����l��1����Ă�̂�-1����)
    memset(tmp, '\0', sizeof(tmp));     //tmp�z��̏�����
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
        MessageBoxA(fp->hwnd, "(13�s��)m_supercut_config.ini�t�@�C���̍\�����Ԉ���Ă��܂�", fp->name, MB_OK);
        return -3;
    }
    //������̏I�[�܂Ń��[�v
    while (buf[cnt] != '\0') {
        if (buf[cnt] == ',') {
            tmp[j] = '\0';
            select9[select_cnt] = atoi(tmp) - 1;    //�ǂݎ�����l�𐮐��ɂ��Ĕz��ɑ��(���C���[�ԍ��ƃv���O������ň����l��1����Ă�̂�-1����)
            memset(tmp, '\0', sizeof(tmp));     //tmp�z��̏�����
            select_cnt++;
            j = 0;
            di_cnt = 0;
            cnt++;
        }
        //4���ȏ�̐������o�Ă��邱�Ƃ͂��肦�Ȃ��̂ŃG���[��f��
        else if (di_cnt >= 4) {
            sprintf_s(buf, sizeof(buf), "(m_supercut_config.ini13�s��%d����)�G���[:4���ȏ�̐������o�Ă��Ă��܂�", cnt + 1);
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
    select9[select_cnt] = atoi(tmp) - 1;    //�ǂݎ�����l�𐮐��ɂ��Ĕz��ɑ��(���C���[�ԍ��ƃv���O������ň����l��1����Ă�̂�-1����)
    memset(tmp, '\0', sizeof(tmp));     //tmp�z��̏�����
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
        MessageBoxA(fp->hwnd, "(14�s��)m_supercut_config.ini�t�@�C���̍\�����Ԉ���Ă��܂�", fp->name, MB_OK);
        return -3;
    }
    //������̏I�[�܂Ń��[�v
    while (buf[cnt] != '\0') {
        if (buf[cnt] == ',') {
            tmp[j] = '\0';
            select10[select_cnt] = atoi(tmp) - 1;    //�ǂݎ�����l�𐮐��ɂ��Ĕz��ɑ��(���C���[�ԍ��ƃv���O������ň����l��1����Ă�̂�-1����)
            memset(tmp, '\0', sizeof(tmp));     //tmp�z��̏�����
            select_cnt++;
            j = 0;
            di_cnt = 0;
            cnt++;
        }
        //4���ȏ�̐������o�Ă��邱�Ƃ͂��肦�Ȃ��̂ŃG���[��f��
        else if (di_cnt >= 4) {
            sprintf_s(buf, sizeof(buf), "(m_supercut_config.ini14�s��%d����)�G���[:4���ȏ�̐������o�Ă��Ă��܂�", cnt + 1);
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
    select10[select_cnt] = atoi(tmp) - 1;    //�ǂݎ�����l�𐮐��ɂ��Ĕz��ɑ��(���C���[�ԍ��ƃv���O������ň����l��1����Ă�̂�-1����)
    memset(tmp, '\0', sizeof(tmp));     //tmp�z��̏�����
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

    //(����)�ŏ��Ɏ��s����镔��
    case WM_FILTER_INIT:
        //�f�[�^�̓ǂݎ��
        set_customselect(hwnd, fp);

        //�g���ҏW0.92�����邩�̔���
        exeditfp = get_exeditfp(fp);
        if (exeditfp == NULL) {
            MessageBoxA(fp->hwnd, "�g���ҏW0.92��������܂���ł���", fp->name, MB_OK);
            break;
        }

        //�V���[�g�J�b�g�L�[�̒ǉ�
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
            //�����t�@�C����������ΐV�K�쐬
            if (_stat("m_supercut_config_inst.txt", &s) != 0) {
                FILE* m_fp;

                //�t�@�C���쐬
                fopen_s(&m_fp, "m_supercut_config_inst.txt", "w");
                if (m_fp == NULL) break;

                //�t�@�C���ɐ�������������
                fprintf(m_fp, "����:config�t�@�C���ɖ��ʂȕ�������s�����Ȃ��ł��������B�����ƃv���O�C��������ɓ��삵�܂���B\n\n");
                fprintf(m_fp, "- �X�[�p�[�J�b�g,�����J�X�^��1�`10�̐ݒ���@ -\n");
                fprintf(m_fp, "config�t�@�C����\" = \"�̌�ɁA�������������C���[�ԍ���S�� �R���}��؂� �ŏ����܂�\n");
                fprintf(m_fp, "���C���[�ԍ��͏���(������������傫����)�ɂȂ�悤�ɏ����Ă�������\n\n\n");
                fprintf(m_fp, "�Ⴆ�΁Acustom1���ȉ��̂悤�ɏ����Aaviutl���j���[�o�[�̕ҏW���X�[�p�[�J�b�g�������J�X�^��1���N���b�N�����(�������͑Ή�����V���[�g�J�b�g�L�[��������)\n");
                fprintf(m_fp, "custom1=1,2,3\n");
                fprintf(m_fp, "���C���[1,2,3�ɂ���I�u�W�F�N�g���Đ��ʒu�ŕ�������܂�\n\n");
                fprintf(m_fp, "�ݒ�ł���̂�custom1�`custom10�܂łł��Acustom11= �Ə����Ă������J�X�^��11���g����Ƃ��������Ƃ͂���܂���B(����Ή�����\���͂���܂���...)\n");

                fclose(m_fp);
            }
            //�t�@�C�����J��
            system("m_supercut_config_inst.txt");
            
            break;
        case MID_FILTER_BUTTON+1:
            //�ݒ�t�@�C�����Ȃ���ΐV�K�쐬
            if (_stat("m_supercut_config.ini", &s) != 0) {
                FILE* m_fp;

                //�t�@�C���쐬
                fopen_s(&m_fp, "m_supercut_config.ini", "w");
                if (m_fp == NULL) break;

                //�t�@�C���ɏ����f�[�^����������
                fprintf(m_fp, "---------------------------------------------------------------------------------------------------\n");
                fprintf(m_fp, "����:���̃t�@�C���ɖ��ʂȕ�������s�����Ȃ��ł��������B�����ƃv���O�C��������ɓ��삵�܂���B\n");
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

            //�ݒ�t�@�C�����J��
            system("m_supercut_config.ini");

            //�ݒ�t�@�C���̏��𔽉f
            set_customselect(hwnd, fp);


            break;
        }
        break;
    }
    return FALSE;
}