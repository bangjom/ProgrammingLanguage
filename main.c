#include <stdio.h>
#include <string.h>
#include <string.h>
#define MAX_STACK_SIZE 1000
#pragma warning(disable: 4996)

////////////////////////////////////////���� ����
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
		printf("������ ���� á���ϴ�.");
	else
		stack[++top] = value;
}

int pop() {
	if (IsEmpty() == 1)
		printf("������ ������ϴ�.");
	else
		return stack[top--];
}

//////////////////////////////////////////�����͸� ���� ���� ����
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
		printf("������ ���� á���ϴ�.");
	else
		cstack[++ctop] = value;
}

char* cpop() {
	if (cIsEmpty() == 1)
		printf("������ ������ϴ�.");
	else
		return cstack[ctop--];
}


//////////////////////////////////////////��ȣ ����
void Eliminate(char* str, char ch)
{
	for (; *str != '\0'; str++)//���� ���ڸ� ���� ������ �ݺ�
	{
		if (*str == ch)//ch�� ���� ������ ��
		{
			strcpy(str, str + 1);
			str--;
		}
	}
}


//////////////////////////////////////////Ʈ�� ����ü
typedef struct _node {
	char* data;
	struct _node* left;
	struct _node* right;
}node;

char* input_table[1000];
static int line_num = 0;
char filename[100];

/////////////////////////////////////////Ʈ�� �ʱ�ȭ
void node_init(node* ptr, char* data) {
	ptr->left = (node*)malloc(sizeof(node));
	ptr->right = (node*)malloc(sizeof(node));
	ptr->data = data;
	cpush(ptr->right);
	cpush(ptr->left);
}

////////////////////////////////////////Ʈ�� ����
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
/////////////////////////////////////////Ʈ�� ����Ž��
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

///////////////////////////////////////////�Լ������
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
		printf("�� �� ��  �� �� �� �� �� >>");
		scanf("%d", &select);
		printf("\n");
		 if(select==1) {
			printf("�� �� �� ��  �� �� �� �� �� >>");
			scanf("%s", filename);
			fileact(filename);
		 }
		 else if (select == 2) {
			 interact();
		 }
		 else if (select == 3) {
			 printf("�� �� �� �� ��  �� �� �� �� ��.");
			 break;
		 }
	}
	return 0;
}

void fileact(char* filename) {
	FILE* fp;
	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("���ϸ��� �����ϴ�.\n");
		return;
	}
	else {
		char buffer[100] = { 0, };
		int  answer[1000] = { 0, };
		while (!feof(fp)) {					//�ٴ����� �޴´�. �޸� �����ϱ����� �����Ҵ�
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
		printf("�� �� �� �� ��\n----------------------------------\n");
		for (int i = 0; i < line_num; i++) {
			printf("%s\n", input_table[i]);
		}
		printf("----------------------------------\n�Դϴ�.\n\n");
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
	printf("\n �� �� ��  �� �� �� �� ��.\n");
	printf("----------------------------------\n");
	getchar();
	scanf("%[^\n]s", str);
	printf("\n----------------------------------\n");
	if (error(str)) return;
	int answer=parser(str);
	printf("----------------------------------\n");
	printf("�� �� : %d\n", answer);
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
	char* temp = calloc(1,strlen(str)+1);	//��ūȭ ��Ű���� �غ�
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

int error(char* str) {	//���� ó��
	char* ptr;
	if (strchr(str, '.')) { 
		printf("�Ǽ� �Է�\n");
		return 1; 
	}
	else if (ptr = strchr(str, '-')) {
		
		ptr = ptr + 1;
		if (ptr[0] == '-') {
			printf("-�����Է�\n");
			return 1;
		}
	}
	else if(!strstr(str,"MINUS")&&!strstr(str,"IF")) {
		printf("undefined\n");
		return 1;
	}

	return 0;
}