#include <stdio.h>
#include <windows.h>
#include <conio.h>

#define strMaxLength 10000		// ��ȣȭ�� ������ �ִ� ���� 

int cryption();
int encryption();

int main(){
	
	char keyS[10]; 	// �޴� ������ ���� ����  
	 

	do {
	printf("\n���װ���׷��� ���α׷��Դϴ�.\n\n");	

		printf("(1) ��ȣȭ    (2) ��ȣȭ   (3)�׳� ������ \n");
		printf("���� : ");
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
			printf("\n\n=> Input Error!!  1, 2, 3 �� �ϳ��� �������ּ���.\n");
			printf("   �ƹ�Ű�� �����ּ���"); 
			getch();
			system("cls");
		}
	}while(keyS[0] != '3' ); 
		
	return 0;
	
}


int cryption(){
	
	BITMAPFILEHEADER bmfh;	// ��Ʈ�� ���� ��� ����  
	BITMAPINFOHEADER hinfo;	// ��Ʈ�� ���� ��� ���� 

	FILE *img, *target;
		
	char sourceFile[20], targetFile[20];	// �ҽ� ���ϸ�, ��� ���ϸ� 
	char str[strMaxLength+1] = "";						// ��ȣȭ�� ���ڿ�  
	unsigned int cryp[strMaxLength*8] = {0, }; 			// ��ȣȭ�� bit�� ������ image data 

	unsigned int check = 0x5555aaaa, ch1, ch2;
	char cS[33] = "01010101010101011010101010101010";	// ��ȣȭ ���� ���� bit ��, 0x5555aaaa(16����) 
	unsigned int len[32] = {0, };	// ��ȣ���̸� ������ �迭 (���ڸ� ������) 
	
	
	unsigned int ch;	// ��Ʈ�� ���Ͽ��� �о� ���� ������ ����  
	int slen;
	
	int i=0, j, n, m, s, s1;
	printf("\n=== ���װ���׷��� ��ȣȭ ===");
	printf("\n\n���� ������ �Է��ϼ���: "); 
	gets(sourceFile);
	printf("��� ������ �Է��ϼ���: ");
	gets(targetFile);
	printf("\n��ȣȭ�� ������ �Է��ϼ���(���� %d�� �̸�) :", strMaxLength);
	gets(str);
	slen = strlen(str);	// ���� ���� Ȯ��
	
	s=0;  // ��ȣȭ�� ������ bit�� (�Է¹������ *8) 
	// ������ ������ str������ ù��° bit���� cryp[0], cryp[1] .. ������ byte�� �� bit�� ����  	
	// slen * 8bit ��ŭ ���� (�Է¹������ * 8) 
	for(n=0; n<slen; n++)
		for(m=7; m>=0 ; m--) {
			cryp[s] = (str[n]>>m) & 1;  
			s++;							
		}

	img = fopen(sourceFile, "rb");
	target = fopen(targetFile, "wb");

	
	fread(&bmfh, sizeof(BITMAPFILEHEADER), 1, img);  // ��Ʈ�� ���� ��� ũ�⸸ŭ �о����
	fread(&hinfo, sizeof(BITMAPINFOHEADER), 1, img);  // ��Ʈ�� ���� ��� ũ�⸸ŭ �о����	
	
	fwrite(&bmfh, sizeof(BITMAPFILEHEADER), 1, target);
	fwrite(&hinfo, sizeof(BITMAPINFOHEADER), 1, target);	
	
	s1=0; 
	i=0; j=31; 
	// ��ȣȭ�� �̹������� ǥ���ϱ� ���� �κ� 		
	while( (ch= fgetc(img)) != EOF)   {   // �̹��� ���� ���� ���� ������ 

		// ��ȣȭ ����, �������, ��ȣȭ�� ���� ����  
		if(i>=0 && i<32) {	// ��ȣȭ �������� ���� 0~31���� 32bit 
			if(cS[i] == '0')      // 1byte �о���� ���� ������ bit�� ������  cS[i]������ ����  
				ch = ch & 0xfffe;	// ������ bit�� 0���� ����� ���� ����  
			else
				ch = ch | 0x0001;	// ������ bit�� 1�� ����� ���� ����  
			i++;
		}else if(i>=32 && i<64) {	// ��ȣȭ���� ���� ����. 32~63���� 32bit 
		// ���� ��ȣȭ ���̸� ����
			len[j] = (slen>>j) & 1; // 1byte ���� ũ���� bit�� �����ϱ����� �迭 ����  
			
			if(len[j] == 0) ch = ch&0xfffe;  // �о���� 1byte�� ������ bit�� ������ ������  
			else			ch = ch|0x0001;
					
			j--;
			i++;
		}else if(i>=64 && i<(64+s) ) {    // 64byte ����. bit�� ����� �Է³����� �о���� �̹���byte���� ����  
			if(cryp[s1] == 0) 	ch = ch&0xfffe;
			else				ch = ch|0x0001;
			
			s1++;
			i++;
		}
				
		// ���Ͽ� ���� (������ ����� ch�� �Ǵ� ������ ����ǴϾ��� ch ��) 
		fprintf(target, "%c", (char)ch);  // 1byte�� ���� 
	}
	
	fclose(img);
	fclose(target);

	
	printf("\n\n��ȣȭ�� �Ϸ�Ǿ����ϴ�.\n");
	printf("�ƹ�Ű�� ���� �ּ���!\n");
	getch();
	system("cls"); 

	return 0;
}

int encryption(){
	
	FILE *img;
	BITMAPFILEHEADER bmfh;	// ��Ʈ�� ���� ��� ����  
	BITMAPINFOHEADER hinfo;
	char readFilename[20];
	int imgData;		// bmp ���Ͽ��� �о���� data  
	char str[strMaxLength+1]="";	// ��ȣ�� �ص��� ������ ������ ���ڿ�  
		
	unsigned int check = 0x5555aaaa;  	// ��ȣȭ ���� �Ǵ� ��,  ������ 01010101010101011010101010101010
	unsigned int chCryp=0, chCryp1=0;  	// ��ȣȭ���θ� �ľ��ϱ����� ����  
	unsigned int chLen=0, chLen1=0;		// ��ȣȭ ���� ũ�⸦ �ľ��ϱ����� ����   
	unsigned int crypData=0;					// ��ȣȭ �� �о���̱� ���� ����
	  
	int i, j;

	printf("\n=== ���װ���׷��� ��ȣȭ ===");
	printf("\n\n�Ǵ� �� �̹��� ������ �Է��ϼ���: "); 
	gets(readFilename);

	img = fopen(readFilename, "rb");
		
	fread(&bmfh, sizeof(BITMAPFILEHEADER), 1, img);  // ��Ʈ�� ���� ��� ũ�⸸ŭ �о����
	fread(&hinfo, sizeof(BITMAPINFOHEADER), 1, img);  // ��Ʈ�� ���� ��� ũ�⸸ŭ �о����	

	i=0; j=7;
	while( (imgData= fgetc(img)) != EOF)   {
		// ��ȣȭ �Ǵ��� �� ����  
		if(i<31) { // chCryp���� ���� ��ȣȭ �Ǵ�  
			chCryp1 = 0x00000001 & imgData;    // imgData ������ bit �� Ȯ���Ͽ� chCryp1�� ����  
			chCryp = (chCryp | chCryp1) << 1;   // chCryp�� �о�� byte�� ������ bit���� ���������� ä�� �ְ� 1bit ���� �̵�  
			i++;
		}else if (i==31) {            // ��ȣȭ �Ǵ� ������ bit���� chCryp�� ä�� ����  
			chCryp1 = 0x00000001 & imgData;
			chCryp = chCryp | chCryp1;
			i++;
			
		}else if (i<63) {  // chLen���� ���� ��ȣȭ ���� ���� �ľ�   
			chLen1 = 0x00000001 & imgData;		
			chLen = (chLen | chLen1) << 1;
			i++;
		}else if(i==63) {
			chLen1 = 0x00000001 & imgData;				
			chLen = chLen | chLen1;		// chLen : ��ȣȭ ���� ���� ����  
			i++;
			
		}else if(i==64){   	// ��ȣȭ �� ������� ù�������� ùbit��, i���� 64�� ��� str[(i-64)/8] ���� ������ ���ܼ� ���� �����Ŵ   
			crypData = 0x00000001 & imgData;
			str[0] = str[0] | ( crypData<<j ) ; // �о���� byte�� �� ���ڸ� bit���� ����				
			i++;
			j--; 
		}else if( i>64 && i<64+(chLen*8) ){
			crypData = 0x00000001 & imgData;	
			str[(i-64)/8] = str[(i-64)/8] | ( crypData<<j);		
			if(j==0) j=7;		// 8bit �� ����ϱ����� (7 ~ 0) 
			else	 j--;
			i++;
		}else if( i>=64+(chLen*8) ) {
			str[chLen+1] = '\0';	// ������ �������� NULL���� ����  
		}
	}
	
	fclose(img);
	
	// ��ȣȭ �Ǵ� ���� ������ ���� ����  
	if(check == chCryp){
		printf("\n�� �׸��� ��о�ȣ�� �ִ� �׸��Դϴ�.\n\n");
		printf("= ������ ��ȣ ====================================\n");		
		printf("%s\n", str);
		printf("==================================================\n");		 
	} else printf("�� �׸��� ��о�ȣ�� �����ϴ�. \n");				
						
	

	printf("\n\n�ƹ�Ű�� �����ּ���!\n");
	getch();
	system("cls"); 

	return 0;
}


