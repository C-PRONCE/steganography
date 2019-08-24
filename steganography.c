#include <stdio.h>
#include <windows.h>
#include <conio.h>

#define strMaxLength 10000		// 암호화할 문장의 최대 길이 

int cryption();
int encryption();

int main(){
	
	char keyS[10]; 	// 메뉴 선택을 위한 변수  
	 

	do {
	printf("\n스테가노그래피 프로그램입니다.\n\n");	

		printf("(1) 암호화    (2) 복호화   (3)그냥 끝내기 \n");
		printf("선택 : ");
		gets(keyS);
		
		if(keyS[0] == '1') {
			system("cls");
			cryption();
			break; 
		}else if(keyS[0] == '2') {
			system("cls");
			encryption();
			break;
		}else if(keyS[0] == '3'){
			break;
		}else{
			printf("\n\n=> Input Error!!  1, 2, 3 중 하나만 선택해주세요.\n");
			printf("   아무키나 눌러주세요"); 
			getch();
			system("cls");
		}
	}while(keyS[0] != '3' ); 
		
	return 0;
	
}


int cryption(){
	
	BITMAPFILEHEADER bmfh;	// 비트맵 파일 헤더 변수  
	BITMAPINFOHEADER hinfo;	// 비트맵 파일 헤더 정보 

	FILE *img, *target;
		
	char sourceFile[20], targetFile[20];	// 소스 파일명, 대상 파일명 
	char str[strMaxLength+1] = "";						// 암호화할 문자열  
	unsigned int cryp[strMaxLength*8] = {0, }; 			// 암호화된 bit를 포함한 image data 

	unsigned int check = 0x5555aaaa, ch1, ch2;
	char cS[33] = "01010101010101011010101010101010";	// 암호화 여부 설정 bit 값, 0x5555aaaa(16진수) 
	unsigned int len[32] = {0, };	// 암호길이를 저장할 배열 (끝자리 이진수) 
	
	
	unsigned int ch;	// 비트맵 파일에서 읽어 값을 저장할 변수  
	int slen;
	
	int i=0, j, n, m, s, s1;
	printf("\n=== 스테가노그래피 암호화 ===");
	printf("\n\n원본 파일을 입력하세요: "); 
	gets(sourceFile);
	printf("대상 파일을 입력하세요: ");
	gets(targetFile);
	printf("\n암호화할 내용을 입력하세요(영문 %d자 미만) :", strMaxLength);
	gets(str);
	slen = strlen(str);	// 문자 길이 확인
	
	s=0;  // 암호화할 문장의 bit수 (입력문장길이 *8) 
	// 저장할 데이터 str문장의 첫번째 bit부터 cryp[0], cryp[1] .. 순서로 byte의 끝 bit에 저장  	
	// slen * 8bit 만큼 실행 (입력문장길이 * 8) 
	for(n=0; n<slen; n++)
		for(m=7; m>=0 ; m--) {
			cryp[s] = (str[n]>>m) & 1;  
			s++;							
		}

	img = fopen(sourceFile, "rb");
	target = fopen(targetFile, "wb");

	
	fread(&bmfh, sizeof(BITMAPFILEHEADER), 1, img);  // 비트맵 파일 헤더 크기만큼 읽어들임
	fread(&hinfo, sizeof(BITMAPINFOHEADER), 1, img);  // 비트맵 파일 헤더 크기만큼 읽어들임	
	
	fwrite(&bmfh, sizeof(BITMAPFILEHEADER), 1, target);
	fwrite(&hinfo, sizeof(BITMAPINFOHEADER), 1, target);	
	
	s1=0; 
	i=0; j=31; 
	// 암호화된 이미지임을 표시하기 위한 부분 		
	while( (ch= fgetc(img)) != EOF)   {   // 이미지 파일 끝을 만날 때까지 

		// 암호화 상태, 문장길이, 암호화된 내용 저장  
		if(i>=0 && i<32) {	// 암호화 상태임을 저장 0~31까지 32bit 
			if(cS[i] == '0')      // 1byte 읽어들일 값에 마지막 bit를 강제로  cS[i]값으로 저장  
				ch = ch & 0xfffe;	// 마지막 bit를 0으로 만들기 위한 연산  
			else
				ch = ch | 0x0001;	// 마지막 bit를 1로 만들기 위한 연산  
			i++;
		}else if(i>=32 && i<64) {	// 암호화문장 길이 저장. 32~63까지 32bit 
		// 내용 암호화 길이를 저장
			len[j] = (slen>>j) & 1; // 1byte 마다 크기의 bit를 저장하기위한 배열 변수  
			
			if(len[j] == 0) ch = ch&0xfffe;  // 읽어들인 1byte의 마지막 bit를 강제로 변경함  
			else			ch = ch|0x0001;
					
			j--;
			i++;
		}else if(i>=64 && i<(64+s) ) {    // 64byte 이후. bit로 변경된 입력내용을 읽어들인 이미지byte값과 연산  
			if(cryp[s1] == 0) 	ch = ch&0xfffe;
			else				ch = ch|0x0001;
			
			s1++;
			i++;
		}
				
		// 파일에 저장 (위에서 변경된 ch값 또는 위에서 변경되니않은 ch 값) 
		fprintf(target, "%c", (char)ch);  // 1byte씩 저장 
	}
	
	fclose(img);
	fclose(target);

	
	printf("\n\n암호화가 완료되었습니다.\n");
	printf("아무키나 눌러 주세요!\n");
	getch();
	system("cls"); 

	return 0;
}

int encryption(){
	
	FILE *img;
	BITMAPFILEHEADER bmfh;	// 비트맵 파일 헤더 변수  
	BITMAPINFOHEADER hinfo;
	char readFilename[20];
	int imgData;		// bmp 파일에서 읽어들인 data  
	char str[strMaxLength+1]="";	// 암호를 해독한 문장을 저장할 문자열  
		
	unsigned int check = 0x5555aaaa;  	// 암호화 여부 판단 값,  이진수 01010101010101011010101010101010
	unsigned int chCryp=0, chCryp1=0;  	// 암호화여부를 파악하기위한 변수  
	unsigned int chLen=0, chLen1=0;		// 암호화 문장 크기를 파악하기위한 변수   
	unsigned int crypData=0;					// 암호화 값 읽어들이기 위한 변수
	  
	int i, j;

	printf("\n=== 스테가노그래피 복호화 ===");
	printf("\n\n판단 할 이미지 파일을 입력하세요: "); 
	gets(readFilename);

	img = fopen(readFilename, "rb");
		
	fread(&bmfh, sizeof(BITMAPFILEHEADER), 1, img);  // 비트맵 파일 헤더 크기만큼 읽어들임
	fread(&hinfo, sizeof(BITMAPINFOHEADER), 1, img);  // 비트맵 파일 헤더 크기만큼 읽어들임	

	i=0; j=7;
	while( (imgData= fgetc(img)) != EOF)   {
		// 암호화 판단할 값 추출  
		if(i<31) { // chCryp값에 의해 암호화 판단  
			chCryp1 = 0x00000001 & imgData;    // imgData 마지막 bit 값 확인하여 chCryp1에 넣음  
			chCryp = (chCryp | chCryp1) << 1;   // chCryp에 읽어온 byte의 마지막 bit값을 순차적으로 채워 넣고 1bit 위로 이동  
			i++;
		}else if (i==31) {            // 암호화 판단 마지막 bit값을 chCryp에 채워 넣음  
			chCryp1 = 0x00000001 & imgData;
			chCryp = chCryp | chCryp1;
			i++;
			
		}else if (i<63) {  // chLen값에 의해 암호화 문장 길이 파악   
			chLen1 = 0x00000001 & imgData;		
			chLen = (chLen | chLen1) << 1;
			i++;
		}else if(i==63) {
			chLen1 = 0x00000001 & imgData;				
			chLen = chLen | chLen1;		// chLen : 암호화 문장 길이 결정  
			i++;
			
		}else if(i==64){   	// 암호화 값 추출시작 첫데이터의 첫bit값, i값이 64일 경우 str[(i-64)/8] 에서 오류가 생겨서 따로 실행시킴   
			crypData = 0x00000001 & imgData;
			str[0] = str[0] | ( crypData<<j ) ; // 읽어들인 byte의 맨 끝자리 bit값을 추출				
			i++;
			j--; 
		}else if( i>64 && i<64+(chLen*8) ){
			crypData = 0x00000001 & imgData;	
			str[(i-64)/8] = str[(i-64)/8] | ( crypData<<j);		
			if(j==0) j=7;		// 8bit 씩 계산하기위함 (7 ~ 0) 
			else	 j--;
			i++;
		}else if( i>=64+(chLen*8) ) {
			str[chLen+1] = '\0';	// 문장의 마지막엔 NULL문자 삽임  
		}
	}
	
	fclose(img);
	
	// 암호화 판단 값과 추출한 값을 비교함  
	if(check == chCryp){
		printf("\n이 그림은 비밀암호가 있는 그림입니다.\n\n");
		printf("= 숨겨진 암호 ====================================\n");		
		printf("%s\n", str);
		printf("==================================================\n");		 
	} else printf("이 그림은 비밀암호가 없습니다. \n");				
						
	

	printf("\n\n아무키나 눌러주세요!\n");
	getch();
	system("cls"); 

	return 0;
}


