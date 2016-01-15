#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <stdlib.h>

char allLine[128];
/*void address_return(FILE* file){
    fgets(add, 64, file);
    int len;
    len = strlen(add);
    add[len-1] = '\0';
}
*/

/*读取门号以及ip地址，分别存入整型变量和数组内
****************************************/
int ip_doorNumber(FILE* doorFd, char* address){
	char doorN[16];
    int i = 0;
    int j = 0;                                                                                                                                  
    if(NULL == doorFd)
	{ 
		perror(" Open files error\n");
		return 1;
	}
    fgets(allLine, 128, doorFd);
    while(allLine[i] != 32)
    {
        address[i] = allLine[i];
        i++;
    }
    address[i++] = '\0';
    while(allLine[i])
    {
        doorN[j] = allLine[i];
        i++;
        j++;
    }

    int doorNumber = atoi(doorN);
	return doorNumber;
}

/*************************************/
int main(void)
{
    char cardNumber[100];
	char add[64];
    char* address_last = ":10080/api/cards";
    int data,doorNum;
    struct curl_slist *list = NULL;
    CURL* geturl;
	int eof;
    FILE* addressFd = fopen("./card_address.conf", "r");
    FILE* cardNumberFd = fopen("./card_number.conf", "r");

    if((NULL == addressFd) && (NULL == cardNumberFd))
	{
		perror("打开文件错误！\n"); 
		return 1; 
	}

    doorNum = ip_doorNumber(addressFd, add);
    geturl = curl_easy_init();
    while(!feof(addressFd))
    {
        strcat(add,address_last);
        printf("\n%s\n", add);
		//从文件中读取卡号，按行读取，一直读到文件末端
        fscanf(cardNumberFd, "%d", &data);
        while(!feof(cardNumberFd))
        {
            sprintf(cardNumber, "{\"cardId\": \"%d\", \"doorCodes\": [\"%d\"]}",data, doorNum);
            printf("\n%s", cardNumber);
            FILE* tmp = fopen("./cardNo", "w+");
            fwrite(cardNumber,1, strlen(cardNumber), tmp);
            fflush(tmp);
            fclose(tmp);
            FILE* tmpd = fopen("./cardNo", "r");
            if(geturl)
            {
                list = curl_slist_append(list, "Content-Type: application/json");
                curl_easy_setopt(geturl, CURLOPT_HTTPHEADER, list);
                curl_easy_setopt(geturl, CURLOPT_URL, add);
                curl_easy_setopt(geturl, CURLOPT_PUT,1L);
                curl_easy_setopt(geturl, CURLOPT_READDATA, tmpd);
                curl_easy_perform(geturl);
                fclose(tmpd);                                                                                                                                                                                        
            }
            fscanf(cardNumberFd, "%d", &data);
        }
		memset(add, '\0', sizeof(add));
    	doorNum = ip_doorNumber(addressFd, add);
		//将文件指针指向开始位置
        fseek(cardNumberFd, 0L, SEEK_SET);
    }
    curl_easy_cleanup(geturl);
    fclose(addressFd);
    fclose(cardNumberFd);
    return 0;
}
