#include <stdio.h>
#include <string.h>
#include <string.h>
#define MAX_STACK_SIZE 1000
#pragma warning(disable: 4996)

////////////////////////////////////////스택 구현
int stack[MAX_STACK_SIZE];
int top = -1;

int IsEmpty() {
	if (top < 0)
		return 1;
	else
		return 0;
}
int IsFull() {
	if (top >= MAX_STACK_SIZE - 1)
		return 1;
	else
		return 0;
}

void push(int value) {
	if (IsFull() == 1)
		printf("스택이 가득 찼습니다.");
	else
		stack[++top] = value;
}

int pop() {
	if (IsEmpty() == 1)
		printf("스택이 비었습니다.");
	else
		return stack[top--];
}

//////////////////////////////////////////포인터를 위한 스택 구현
char* cstack[MAX_STACK_SIZE];
int ctop = -1;

int cIsEmpty() {
	if (ctop < 0)
		return 1;
	else
		return 0;
}
int cIsFull() {
	if (ctop >= MAX_STACK_SIZE - 1)
		return 1;
	else
		return 0;
}

void cpush(char* value) {
	if (cIsFull() == 1)
		printf("스택이 가득 찼습니다.");
	else
		cstack[++ctop] = value;
}

char* cpop() {
	if (cIsEmpty() == 1)
		printf("스택이 비었습니다.");
	else
		return cstack[ctop--];
}


//////////////////////////////////////////괄호 삭제
void Eliminate(char* str, char ch)
{
	for (; *str != '\0'; str++)//종료 문자를 만날 때까지 반복
	{
		if (*str == ch)//ch와 같은 문자일 때
		{
			strcpy(str, str + 1);
			str--;
		}
	}
}


//////////////////////////////////////////트리 구조체
typedef struct _node {
	char* data;
	struct _node* left;
	struct _node* right;
}node;

char* input_table[1000];
static int line_num = 0;
char filename[100];

/////////////////////////////////////////트리 초기화
void node_init(node* ptr, char* data) {
	ptr->left = (node*)malloc(sizeof(node));
	ptr->right = (node*)malloc(sizeof(node));
	ptr->data = data;
	cpush(ptr->right);
	cpush(ptr->left);
}

////////////////////////////////////////트리 삽입
void node_insert(node* ptr, char* data) {

	if (strcmp(data,"MINUS")==0 || strcmp(data,"IF")==0) {
		ptr->left = (node*) malloc(sizeof(node));
		ptr->right = (node*) malloc(sizeof(node));
		ptr->data = data;
		cpush(ptr->right);
		cpush(ptr->left);
	}
	else {
		ptr->left = ptr->right = NULL;
		ptr->data = data;
	}
}
/////////////////////////////////////////트리 후위탐색
void postorder(node* ptr) {
	if (ptr) {
		postorder(ptr->left); 
		postorder(ptr->right);
		if (strstr(ptr->data, "MINUS")) {
			int b = pop();
			int a = pop();
			int temp;
			temp = a - b;
			push(temp);
			
		}
		else if (strstr(ptr->data, "IF")) {
			int b = pop();
			int a = pop();
			if (a > 0) push(b);
			else push(0);
		}
		else {
			if (strchr(ptr->data, '-')) {
				Eliminate(ptr->data, '-');
				push(-1*atoi(ptr->data));
			}
			else {
				push(atoi(ptr->data));
			}
		}
	}
}

///////////////////////////////////////////함수선언부
void fileact(char*);
void interact(void);
int parser(char*);
int error(char*);



int main(void) {

	int select = 0;
	
	while (1) {
		printf("==================================\n");
		printf("1. File Load\n");
		printf("2. Interactive Mode\n");
		printf("3. Exit\n");
		printf("==================================\n");
		printf("메 뉴 를  선 택 하 세 요 >>");
		scanf("%d", &select);
		printf("\n");
		 if(select==1) {
			printf("파 일 명 을  입 력 하 세 요 >>");
			scanf("%s", filename);
			fileact(filename);
		 }
		 else if (select == 2) {
			 interact();
		 }
		 else if (select == 3) {
			 printf("프 로 그 램 을  종 료 합 니 다.");
			 break;
		 }
	}
	return 0;
}

void fileact(char* filename) {
	FILE* fp;
	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("파일명이 없습니다.\n");
		return;
	}
	else {
		char buffer[100] = { 0, };
		int  answer[1000] = { 0, };
		while (!feof(fp)) {					//줄단위로 받는다. 메모리 절약하기위해 동적할당
			fgets(buffer, sizeof(buffer), fp);
			input_table[line_num] = calloc(1, sizeof(buffer));
			strcpy(input_table[line_num],buffer);
			line_num++;
		}
		fclose(fp);
		
		for (int i = 0; i < line_num; i++) {
			if (error(input_table[i])) goto gg;
			answer[i] = parser(input_table[i]);
		}
		printf("파 일 내 용 은\n----------------------------------\n");
		for (int i = 0; i < line_num; i++) {
			printf("%s\n", input_table[i]);
		}
		printf("----------------------------------\n입니다.\n\n");
		for (int i = 0; i < line_num; i++) {
			printf("%d\n", answer[i]);
		}
		for (int i = 0; i < line_num; i++) {
			free(input_table[i]);
		}
	}
gg:	
	return;
}

void interact(void) {
	char str[100];
	printf("\n 문 장 을  입 력 하 세 요.\n");
	printf("----------------------------------\n");
	getchar();
	scanf("%[^\n]s", str);
	printf("\n----------------------------------\n");
	if (error(str)) return;
	int answer=parser(str);
	printf("----------------------------------\n");
	printf("결 과 : %d\n", answer);
	printf("\n----------------------------------\n");
	return;
}

int parser(char* str) {
	while (top != -1)
		pop();
	while (ctop != -1)
		cpop();
	int i = 1;
	int cnt = 0;
	node* root=(node*)malloc(sizeof(node));
	char* temp = calloc(1,strlen(str)+1);	//토큰화 시키전에 준비
	strcpy(temp, str);
	Eliminate(temp, '(');
	Eliminate(temp, ')');
	
	char* ptr = strtok(temp, " ");
	node_init(root, ptr);
	while (ptr != NULL) {
		ptr = strtok(NULL, " ");
		if (ptr == NULL) break;
		node_insert(cpop(), ptr);  
		
	}
	postorder(root);
	return pop();
}

int error(char* str) {	//에러 처리
	char* ptr;
	if (strchr(str, '.')) { 
		printf("실수 입력\n");
		return 1; 
	}
	else if (ptr = strchr(str, '-')) {
		
		ptr = ptr + 1;
		if (ptr[0] == '-') {
			printf("-다중입력\n");
			return 1;
		}
	}
	else if(!strstr(str,"MINUS")&&!strstr(str,"IF")) {
		printf("undefined\n");
		return 1;
	}

	return 0;
}