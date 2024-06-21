#include "UID_Encryption.h"

/*
V1.2:
    
    ���£�
        ����������Կ����Ϊ4�ֽڡ�8�ֽ�ʱ��ȡ�û��Զ��� IDԽ�絼���������Կ�������ȷ�����⡣
        MINI-Pro�ѻ������� V1.20B �汾�̼���ʹ��V1.2�汾���룻
        MINI�ѻ������� V1.4A2 �汾�̼���ʹ��V1.2�汾���룻
        
    Ӱ�죺
        ǰ��������
            (1).MINI���ѻ��������������˹̼���V1.4A2��ʹ��MINI�ѻ����������õ���V1.2�汾����;
            (2).MINI-Pro���ѻ��������������˹̼���V1.20B��ʹ��MINI�ѻ����������õ���V1.2�汾����;
            (3).��Կ��������Ϊ4�ֽڡ�8�ֽڵ������
        ��������
            (1).�����������㷨����Կ����Ϊ 4/8�ֽ�ʱ�������߼����ᵼ�����������Կ���������Կ����Ϊ4/8�ֽ���Ӱ�죩
            ��V1.1�汾ǰ�Ĵ�����������ͬ��
        ����취��
            (1).MINI-Pro���ѻ��������Ѿ�������V1.20B�汾�̼����û����Ի��˵��ɰ汾�̼�����ʹ�á�
            (2).MINI���ѻ��������Ѿ�������V1.4A2�汾�̼����û����Ի��˵��ɰ汾�̼�����ʹ�á�
        ����Ӱ��������
            (1).ֻҪ��Կ����Ϊ12�ֽھ�����Ӱ�죻
V1.1:
    
    ���£�
        MINI���ѻ�������V1,3A5��̼�ʹ��V1.1�汾���룬V1.1�汾�����޸���V1.0�汾
        ����UID_Encryption_Key_Check(),UID_Encryption_Key_Calculate()ʹ�ô��
        ģʽ�п��ܳ��ֵ��ڴ�Խ�����⡣
    Ӱ�죺
        ǰ��������
            (1).��������MINI���ѻ��������Ĺ̼���V1.3A5��ʹ��MINI�ѻ����������õ���V1.1�汾����;
            (2).��ǰ ʹ��V1.3A4��֮ǰ�汾�̼���MINI�ѻ������� ����Ϊ���ģʽ�����û�����ʹ�õ��������V1.0�汾���룻
        ��������
            (1).����ʹ��V1.3A5�汾�̼���MINI�ѻ���������������UID�Զ�����ܹ��ܣ�Ȼ�������¼������V1.0�汾������û�����
            ����V1.1�汾����Ĵ��ģʽ��Կ��V1.0�汾�������Կ��һ�£���˻ᵼ��оƬ�л���ֻ������Կ��֤��ͨ�������⣡
        ����취��
            (1).���û������е�V1.0�����ͬ�����µ�V1.1����롣
        ����Ӱ��������
            (1).��ǰʹ�� С��ģʽ ���õ��û���Ʒ�����ܵ�V1.1�汾��������Լ�MINI�ѻ��������̼����µ�Ӱ�죻
            (2).����Ѿ�ʹ��V1.0����Ĳ�Ʒ����ʹ��IAP��ʽ�����򲻻��ܵ�Ӱ�죻
            (3).����Ѿ�ʹ��V1.0����Ĳ�Ʒ����ʹ��MINI�ѻ�������������Ʒ �� ������UID�Զ�����ܹ������¸�����Կ�����ܵ�Ӱ�죬
            �������ʹ���ѻ�������������Ʒ�Ļ�����������û�������˵�UID�Զ�����ܴ���ΪV1.1��ͬʱ����UID�Զ���������¸�����Կ��
            (4)MINI-Pro���ѻ��������װ�̼���ʹ�õ���V1.1�汾�Ĵ��룬����Ӱ�죻
V1.0��
    MINI���ѻ�������V1.3A4��̼���֮ǰ�Ĺ̼��о�ʹ����V1.0�汾�Ĵ��룻
*/

   
/****************************************** UID�����㷨 start ******************************************/
/* UID�����㷨0 */
static void Algorithm_0(char *pCustomID, char *pUID, char KeyLength, char *pKey)
{
    pKey[0] = (pUID[0]    ^ (pCustomID[3] & pCustomID[2])) ^ pCustomID[1];
    pKey[1] = pCustomID[1] | (pUID[2]    ^ (pCustomID[3] & pCustomID[3]));
    pKey[2] = (pCustomID[2] ^ pCustomID[1]) | (pUID[0]    ^ pCustomID[0]);
    pKey[3] = ((pCustomID[3] & pCustomID[0]) ^ pCustomID[1]) | pUID[2];
    
    if(KeyLength == 8)
    {
        pKey[4] = (pUID[4]    ^ (pCustomID[7] & pCustomID[5])) ^ pCustomID[5];
        pKey[5] = pCustomID[5] | (pUID[6]    ^ (pCustomID[4] & pCustomID[7]));
        pKey[6] = (pCustomID[6] ^ pCustomID[5]) | (pUID[7]    ^ pCustomID[4]);
        pKey[7] = ((pCustomID[7] & pCustomID[4]) ^ pCustomID[6]) | pUID[6];
    }
    if(KeyLength == 12)
    {
        pKey[0] = (pUID[0]    ^ (pCustomID[1] & pCustomID[2])) ^ pCustomID[3];
        pKey[1] = pCustomID[1] | (pUID[1]    ^ (pCustomID[3] & pCustomID[4]));
        pKey[2] = (pCustomID[2] ^ pCustomID[1]) | (pUID[2]    ^ pCustomID[5]);
        pKey[3] = ((pCustomID[3] & pCustomID[1]) ^ pCustomID[5]) | pUID[3];
        pKey[4] = (pUID[4]    ^ (pCustomID[5] & pCustomID[6])) ^ pCustomID[7];
        pKey[5] = pCustomID[5] | (pUID[5]    ^ (pCustomID[7] & pCustomID[8]));
        pKey[6] = (pCustomID[6] ^ pCustomID[7]) | (pUID[6]    ^ pCustomID[9]);
        pKey[7] = ((pCustomID[7] & pCustomID[8]) ^ pCustomID[9]) | pUID[7];
        pKey[8] = (pUID[8]    ^ (pCustomID[9] & pCustomID[10])) ^ pCustomID[11];
        pKey[9] = pCustomID[9] | (pUID[9]    ^ (pCustomID[11] & pCustomID[0]));
        pKey[10]= (pCustomID[10] ^ pCustomID[11]) | (pUID[0]   ^ pCustomID[1]);
        pKey[11]= ((pCustomID[11] & pCustomID[0]) ^ pCustomID[1]) | pUID[2];
    }
       
}



/* UID�����㷨1 */
static void Algorithm_1(char *pCustomID, char *pUID, char KeyLength, char *pKey)
{
    pKey[0] = ((pUID[0]    ^ pCustomID[1]) ^ pCustomID[2]) ^ pCustomID[3];
    pKey[1] = ((pCustomID[1] ^ pUID[1])    ^ pCustomID[3]) ^ pCustomID[0];
    pKey[2] = ((pCustomID[2] ^ pCustomID[1]) ^ pUID[2])    ^ pCustomID[3];
    pKey[3] = ((pCustomID[3] ^ pCustomID[1]) ^ pCustomID[0]) ^ pUID[3];
    
    if(KeyLength == 8)
    {
        pKey[4] = ((pUID[4]    ^ pCustomID[5]) ^ pCustomID[6]) ^ pCustomID[7];
        pKey[5] = ((pCustomID[5] ^ pUID[5])    ^ pCustomID[7]) ^ pCustomID[2];
        pKey[6] = ((pCustomID[6] ^ pCustomID[7]) ^ pUID[6])    ^ pCustomID[3];
        pKey[7] = ((pCustomID[7] ^ pCustomID[4]) ^ pCustomID[5]) ^ pUID[4];
    }
    if(KeyLength == 12)
    {
        pKey[0] = ((pUID[0]    ^ pCustomID[1]) ^ pCustomID[2]) ^ pCustomID[3];
        pKey[1] = ((pCustomID[1] ^ pUID[1])    ^ pCustomID[3]) ^ pCustomID[4];
        pKey[2] = ((pCustomID[2] ^ pCustomID[1]) ^ pUID[2])    ^ pCustomID[5];
        pKey[3] = ((pCustomID[3] ^ pCustomID[1]) ^ pCustomID[5]) ^ pUID[3];
        pKey[4] = ((pUID[4]    ^ pCustomID[5]) ^ pCustomID[6]) ^ pCustomID[7];
        pKey[5] = ((pCustomID[5] ^ pUID[5])    ^ pCustomID[7]) ^ pCustomID[8];
        pKey[6] = ((pCustomID[6] ^ pCustomID[7]) ^ pUID[6])    ^ pCustomID[9];
        pKey[7] = ((pCustomID[7] ^ pCustomID[8]) ^ pCustomID[9]) ^ pUID[7];
        pKey[8] = ((pUID[8]    ^ pCustomID[9]) ^ pCustomID[10]) ^ pCustomID[11];
        pKey[9] = ((pCustomID[9] ^ pUID[9])    ^ pCustomID[11]) ^ pCustomID[0];
        pKey[10]= ((pCustomID[10] ^ pCustomID[11]) ^ pUID[0])   ^ pCustomID[1];
        pKey[11]= ((pCustomID[11] ^ pCustomID[0]) ^ pCustomID[1]) ^ pUID[2];
    }
}


/* UID�����㷨2 */
static void Algorithm_2(char *pCustomID, char *pUID, char KeyLength, char *pKey)
{
    pKey[0] = ((pUID[0]    & pCustomID[1]) ^ pCustomID[2]) ^ pCustomID[3];
    pKey[1] = (pCustomID[1] ^ (pUID[1]    & pCustomID[3])) ^ pCustomID[0];
    pKey[2] = (pCustomID[2] | (pCustomID[1] ^ pUID[2]))    | pCustomID[3];
    pKey[3] = (pCustomID[3] ^ pCustomID[1]) | (pCustomID[0] ^ pUID[3]);
    
    if(KeyLength == 8)  
    {
        pKey[4] = (pUID[4]    ^ (pCustomID[5] & pCustomID[6])) ^ pCustomID[7];
        pKey[5] = pCustomID[5] | (pUID[5]    ^ (pCustomID[7] & pCustomID[2]));
        pKey[6] = ((pCustomID[6] & pCustomID[7]) ^ pUID[6])    | pCustomID[3];
        pKey[7] = (pCustomID[7] ^ pCustomID[4]) | (pCustomID[1] ^ pUID[2]);
    }
    if(KeyLength == 12)
    {
        pKey[0] = ((pUID[0]    & pCustomID[1]) ^ pCustomID[2]) ^ pCustomID[3];
        pKey[1] = (pCustomID[1] ^ (pUID[1]    & pCustomID[3])) ^ pCustomID[4];
        pKey[2] = (pCustomID[2] | (pCustomID[1] ^ pUID[2]))    | pCustomID[5];
        pKey[3] = (pCustomID[3] ^ pCustomID[1]) | (pCustomID[5] ^ pUID[3]);
        pKey[4] = (pUID[4]    ^ (pCustomID[5] & pCustomID[6])) ^ pCustomID[7];
        pKey[5] = pCustomID[5] | (pUID[5]    ^ (pCustomID[7] & pCustomID[8]));
        pKey[6] = ((pCustomID[6] & pCustomID[7]) ^ pUID[6])    | pCustomID[9];
        pKey[7] = (pCustomID[7] ^ pCustomID[8]) | (pCustomID[9] ^ pUID[7]);
        pKey[8] = (pUID[8]    ^ (pCustomID[9] & pCustomID[10])) ^ pCustomID[11];
        pKey[9] = (pCustomID[9] ^ pUID[9])    | (pCustomID[11] & pCustomID[0]);
        pKey[10]= ((pCustomID[10] & pCustomID[11]) ^ pUID[0])   ^ pCustomID[1];
        pKey[11]= (pCustomID[11] ^ (pCustomID[0] & pCustomID[1])) ^ pUID[2];
    }
}

/* UID�����㷨3 */
static void Algorithm_3(char *pCustomID, char *pUID, char KeyLength, char *pKey)
{
    pKey[0] = ((pUID[0] & pCustomID[1]) ^ pCustomID[2]) ^ pCustomID[3];
    pKey[1] = (pUID[1] ^ (pCustomID[2] & pCustomID[3])) ^ pCustomID[4];
    pKey[2] = (pUID[2] | (pCustomID[3] ^ pCustomID[4])) | pCustomID[5];
    pKey[3] = (pUID[3] ^ pCustomID[4]) | (pCustomID[5] ^ pCustomID[6]);
    
    if(KeyLength == 8)  
    {
        pKey[4] = (pCustomID[4] ^ (pUID[5] & pCustomID[6])) ^ pCustomID[7];
        pKey[5] = pCustomID[5] | (pUID[6] ^ (pCustomID[7] & pCustomID[4]));
        pKey[6] = ((pCustomID[6] & pUID[7]) ^ pCustomID[6]) | pCustomID[3];
        pKey[7] = (pCustomID[7] ^ pUID[4]) | (pCustomID[5] ^ pCustomID[6]);
    }
    if(KeyLength == 12)
    {
        pKey[0] = ((pUID[0] & pCustomID[1]) ^ pCustomID[2]) ^ pCustomID[3];
        pKey[1] = (pUID[1] ^ (pCustomID[2] & pCustomID[3])) ^ pCustomID[4];
        pKey[2] = (pUID[2] | (pCustomID[3] ^ pCustomID[4])) | pCustomID[5];
        pKey[3] = (pUID[3] ^ pCustomID[4]) | (pCustomID[5] ^ pCustomID[6]);
        pKey[4] = (pCustomID[4] ^ (pUID[5] & pCustomID[6])) ^ pCustomID[7];
        pKey[5] = pCustomID[5] | (pUID[6] ^ (pCustomID[7] & pCustomID[8]));
        pKey[6] = ((pCustomID[6] & pUID[7]) ^ pCustomID[6]) | pCustomID[9];
        pKey[7] = (pCustomID[7] ^ pUID[8]) | (pCustomID[9] ^ pCustomID[10]);
        pKey[8] = (pCustomID[8]  ^ (pCustomID[9]  & pUID[10])) ^ pCustomID[11];
        pKey[9] = (pCustomID[9]  ^ pCustomID[10]) | (pUID[11] & pCustomID[0]);
        pKey[10]= ((pCustomID[10] & pCustomID[11]) ^ pUID[0])  ^ pCustomID[1];
        pKey[11]= (pCustomID[11] ^ (pCustomID[0]  & pUID[1]))  ^ pCustomID[2];
    }
}


/* UID�����㷨4 */
static void Algorithm_4(char *pCustomID, char *pUID, char KeyLength, char *pKey) 
{
    pKey[0] = (pUID[0] & pCustomID[1]) ^ (pUID[2] & pCustomID[3]);
    pKey[1] = (pCustomID[1] ^ (pUID[2] & pCustomID[3])) ^ pUID[4];
    pKey[2] = (pCustomID[2] | (pUID[3] ^ pUID[4])) | pCustomID[3];
    pKey[3] = (pUID[3] - pCustomID[0]) | (pCustomID[2] ^ pUID[3]);
    
    if(KeyLength == 8)  
    {
        pKey[4] = (pUID[4] ^ (pCustomID[5] & pUID[6])) ^ pCustomID[7];
        pKey[5] = pCustomID[5] | (pUID[6] ^ (pCustomID[7] & pUID[8]));
        pKey[6] = ((pCustomID[6] & pUID[7]) ^ pCustomID[6]) | pUID[9];
        pKey[7] = (pUID[7] - pCustomID[3]) | (pUID[9] ^ pCustomID[5]);
    }
    if(KeyLength == 12)
    {
        pKey[0] = (pUID[0] & pCustomID[1]) ^ (pUID[2] & pCustomID[3]);
        pKey[1] = (pCustomID[1] ^ (pUID[2] & pCustomID[3])) ^ pUID[4];
        pKey[2] = (pCustomID[2] | (pUID[3] ^ pUID[4])) | pCustomID[5];
        pKey[3] = (pUID[3] - pCustomID[4]) | (pCustomID[5] ^ pUID[6]);
        pKey[4] = (pUID[4] ^ (pCustomID[5] & pUID[6])) ^ pCustomID[7];
        pKey[5] = pCustomID[5] | (pUID[6] ^ (pCustomID[7] & pUID[8]));
        pKey[6] = ((pCustomID[6] & pUID[7]) ^ pCustomID[6]) | pUID[9];
        pKey[7] = (pUID[7] - pCustomID[8]) | (pUID[9] ^ pCustomID[10]);
        pKey[8] = (pCustomID[8] ^ (pUID[9] & pCustomID[10])) ^ pUID[11];
        pKey[9] = (pUID[9] ^ pCustomID[10]) | (pUID[11] & pCustomID[0]);
        pKey[10]= ((pUID[10] & pCustomID[11]) ^ pUID[0])  ^ pCustomID[1];
        pKey[11]= ((pCustomID[11] - pUID[0]) & pCustomID[1])  ^ pUID[2];
    }
}



/* ����UID�����㷨 ����ָ������ */
typedef void (*Algorithm_Fun_Typedef)(char *pCustomID, char *pUID, char KeyLength, char *pKey);
/* ����UID�����㷨 ����ָ������ */
static const Algorithm_Fun_Typedef Algorithm_Fun_Array[5] = 
{
    Algorithm_0,
    Algorithm_1,
    Algorithm_2,
    Algorithm_3,
    Algorithm_4,
};
/****************************************** UID�����㷨 end ******************************************/    



/* ��С�����л� */      
#define BigLittleSwap32(A)          ((((unsigned int)(A) & 0xFF000000) >> 24) | \
                                     (((unsigned int)(A) & 0x00FF0000) >> 8)  | \
                                     (((unsigned int)(A) & 0x0000FF00) << 8)  | \
                                     (((unsigned int)(A) & 0x000000FF) << 24)) 
                                        
void LittleEndia_BigEndia_Interconvert_32(unsigned int* pBuffer, unsigned int WordLen)
{
    unsigned int i;
    
    for(i = 0; i < WordLen; i ++)
    {
        pBuffer[i] = BigLittleSwap32(pBuffer[i]);
    }
    return;
}  



static Algorithm_Fun_Typedef   pAlgorithm_Fun;
/****************** UID �㷨������Կ��֤ ******************/
char UID_Encryption_Key_Check(void *pKey,
                              void *pUID,
                              void *pCustomID,
                              eKeyLengthType keyLength, 
                              eEndiaType endiaType, 
                              eAlgorithmType AlgorithmNum)
{   
    char KeyBuf[12] = {0};//��ʱ�洢����õ���Key
    
    
    /* ѡ���㷨��ʽ */
    pAlgorithm_Fun = Algorithm_Fun_Array[AlgorithmNum];
    /* ����KEY */
    (*pAlgorithm_Fun)(pCustomID, pUID, keyLength, KeyBuf);   

    if(endiaType == BIG_ENDIA)
    {
        LittleEndia_BigEndia_Interconvert_32((unsigned int*)KeyBuf, keyLength/sizeof(unsigned int));
    }
    /* �Ƚ��ѻ���������¼��Key��ʹ����ͬ�㷨����õ���key�Ƿ���ͬ */
    if(memcmp(pKey, KeyBuf, keyLength)) 
    {
        return 1;
    }
    
    return 0; 
}
/****************** UID �㷨������Կ���� ******************/
void UID_Encryption_Key_Calculate(void *pKey,
                                 void *pUID,
                                 void *pCustomID,
                                 eKeyLengthType keyLength, 
                                 eEndiaType endiaType, 
                                 eAlgorithmType AlgorithmNum)
{    
    /* ѡ���㷨��ʽ */
    pAlgorithm_Fun = Algorithm_Fun_Array[AlgorithmNum];
    /* ����KEY */
    (*pAlgorithm_Fun)(pCustomID, pUID, keyLength, pKey);   

    if(endiaType == BIG_ENDIA)
    {
        LittleEndia_BigEndia_Interconvert_32((unsigned int*)pKey, keyLength/sizeof(unsigned int));
    }
    
    return; 
}








