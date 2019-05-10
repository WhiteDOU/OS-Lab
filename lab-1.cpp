#include<stdio.h>
#include<stdlib.h>
#include<sys/ioctl.h>

#define ElementType cpb
#define MAXSIZE 30
#define ERROR -99
#define MEMORIZE 102

typedef struct {
	char name;
	int mem_size;	//占用内存大小
	int pri;	//优先级
	int time;	//运行周期（次）
}cpb;

typedef struct {	//定义队列结构体
	ElementType data[MAXSIZE];
	int front;
	int rear;
	int size;
}Queue;

Queue* createQ() {	//创建队列
	Queue* q = (Queue*)malloc(sizeof(Queue));
	q->front = -1;
	q->rear = -1;
	q->size = 0;
	return q;
}

int is_emptyQ(Queue* q) {	//判断队列是否为空
	return(q->size == 0);
}

void addQ(Queue* q, ElementType item) {	//入队列
	q->rear++;
	q->size++;
	q->data[q->rear] = item;
}

ElementType deleteQ(Queue* q) {	//出队列
	q->front++;
	q->size--;
	return q->data[q->front];
}

void printQ(Queue* q) {	//输出队列元素
	if (is_emptyQ(q)) {
		printf("  ");
		return;
	}
	int index = q->front;
	int i;
	for (i = 0; i < q->size; i++) {
		index++;
		printf("%c\t", (q->data[index]).name);
	}
}
void transfer2(Queue* runningq, Queue* readyq, Queue* blockedq, Queue* createq) {
	int em1, em2;
	ElementType a, b;
	em1 = is_emptyQ(runningq);
	em2 = is_emptyQ(readyq);
	if ((em1 == 0) && (em2 == 0))
	{
		a = deleteQ(runningq);
		addQ(readyq, a);
		b = deleteQ(readyq);
		addQ(runningq, b);
	}
	else if ((em1 == 0) && (em2 != 0))
	{
		a = deleteQ(runningq);
		addQ(readyq, a);
	}
	else if ((em1 != 0) && (em2 == 0))
	{
		b = deleteQ(readyq);
		addQ(runningq, b);
	}
	else {}
	printf("转换后的状态为：\n");
	printf("Running:\t");
	printQ(runningq);
	printf("\n");
	printf("Ready:\t\t");
	printQ(readyq);
	printf("\n");
	printf("Blocked:\t");
	printQ(blockedq);
	printf("\n");
	printf("Create:\t");
	printQ(createq);
	printf("\n");
	printf("\n");
}

void transfer3(Queue* runningq, Queue* readyq, Queue* blockedq, Queue* createq) {
	int em1, em2;
	ElementType a, b;
	em1 = is_emptyQ(runningq);
	em2 = is_emptyQ(readyq);
	if ((em1 == 0) && (em2 == 0))
	{
		a = deleteQ(runningq);
		addQ(blockedq, a);
		b = deleteQ(readyq);
		addQ(runningq, b);
	}
	else if ((em1 == 0) && (em2 != 0))
	{
		a = deleteQ(runningq);
		addQ(blockedq, a);
	}
	else if ((em1 != 0) && (em2 == 0))
	{
		b = deleteQ(readyq);
		addQ(runningq, b);
	}
	else {
		ElementType temp = deleteQ(blockedq);
		addQ(runningq, temp);
	}
	printf("转换后的状态为：\n");
	printf("Running:\t");
	printQ(runningq);
	printf("\n");
	printf("Ready:\t\t");
	printQ(readyq);
	printf("\n");
	printf("Blocked:\t");
	printQ(blockedq);
	printf("\n");
	printf("Create:\t");
	printQ(createq);
	printf("\n");
	printf("\n");
}

void transfer4(Queue* runningq, Queue* readyq, Queue* blockedq, Queue* createq) {
	int em1, em3;
	ElementType a, b;
	em1 = is_emptyQ(runningq);
	em3 = is_emptyQ(blockedq);
	if (em3 == 0)
	{
		a = deleteQ(blockedq);
		addQ(readyq, a);
		if (em1 != 0)
		{
			b = deleteQ(readyq);
			addQ(runningq, b);
		}
	}
	printf("转换后的状态为：\n");
	printf("Running:\t");
	printQ(runningq);
	printf("\n");
	printf("Ready:\t\t");
	printQ(readyq);
	printf("\n");
	printf("Blocked:\t");
	printQ(blockedq);
	printf("\n");
	printf("Create:\t");
	printQ(createq);
	printf("\n");
	printf("\n");
}

cpb transfer5(Queue* runningq, Queue* readyq, Queue* blockedq, Queue* createq, int size_now) {
	//char mmm[]="PQRSTUVWXYZ"
	char name;
	int size, pri, time;
	printf("name:");
	scanf(" %c",&name);
	printf("size:");
	scanf("%d", &size);
	printf("priority:");
	scanf("%d", &pri);
	printf("time:");
	scanf("%d", &time);
	cpb process = { name,size,pri,time };
	size_now = size_now + process.mem_size;
	if (size_now > MEMORIZE)
	{
		printf("Process will be waiting.\n");
		addQ(createq, process);
	}
	else if (is_emptyQ(runningq) != 0)
	{
		addQ(runningq, process);
	}
	else
	{
		addQ(readyq, process);
	}
	printf("转换后的状态为：\n");
	printf("Running:\t");
	printQ(runningq);
	printf("\n");
	printf("Ready:\t\t");
	printQ(readyq);
	printf("\n");
	printf("Blocked:\t");
	printQ(blockedq);
	printf("\n");
	printf("Create:\t");
	printQ(createq);
	printf("\n");
	printf("\n");
	return(process);
}

int transfer6(Queue* runningq, Queue* readyq, Queue* blockedq, Queue* createq, int size_now)
{
	ElementType a,b,c;
	if (is_emptyQ(runningq) == 0)
	{
		a=deleteQ(runningq);
		if (is_emptyQ(readyq) == 0)
		{
			b = deleteQ(readyq);
			addQ(runningq, b);
		}
		else {}
	}
	size_now = size_now - a.mem_size;
	if ((size_now < MEMORIZE)&&(is_emptyQ(createq) == 0))
	{
		c= deleteQ(createq);
		addQ(readyq, c);
	}
	printf("转换后的状态为：\n");
	printf("Running:\t");
	printQ(runningq);
	printf("\n");
	printf("Ready:\t\t");
	printQ(readyq);
	printf("\n");
	printf("Blocked:\t");
	printQ(blockedq);
	printf("\n");
	printf("Create:\t");
	printQ(createq);
	printf("\n");
	printf("\n");
	return(size_now);
}

int main()
{
	int trans, size_now, pro_num = 15;
	Queue* runningq = createQ();	//运行态
	Queue* readyq = createQ();	//就绪态
	Queue* blockedq = createQ();	//阻塞态
	Queue* createq = createQ();	//创建新进程，若内存已满，则存入
	cpb process[30];
	process[0].name='A';
	process[0].mem_size=6;

	process[1].name='B';
	process[1].mem_size=5;
	process[2].name='C';
	process[2].mem_size=6;
	process[3].name='D';
	process[3].mem_size=6;
	process[4].name='E';
	process[4].mem_size=6;
	process[5].name='F';
	process[5].mem_size=6;
	process[6].name='G';
	process[6].mem_size=5;
	process[7].name='H';
	process[7].mem_size=6;
	process[8].name='I';
	process[8].mem_size=6;
	process[9].name='J';
	process[9].mem_size=6;
	process[10].name='K';
	process[10].mem_size=6;
	process[11].name='L';
	process[11].mem_size=5;
	process[12].name='M';
	process[12].mem_size=6;
	process[13].name='N';
	process[13].mem_size=6;
	process[14].name='O';
	process[14].mem_size=6;

	addQ(runningq, process[0]);
	addQ(readyq, process[1]);
	addQ(readyq, process[2]);
	addQ(readyq, process[3]);
	addQ(readyq, process[4]);
	addQ(readyq, process[5]);
	addQ(readyq, process[6]);
	addQ(readyq, process[7]);
	addQ(blockedq, process[8]);
	addQ(blockedq, process[9]);
	addQ(blockedq, process[10]);
	addQ(blockedq, process[11]);
	addQ(blockedq, process[12]);
	addQ(blockedq, process[13]);
	addQ(blockedq, process[14]);
	size_now = 82;
	printf("当前进程状态：\n");
	printf("Running:\t");
	printQ(runningq);
	printf("\n");
	printf("Ready:\t\t");
	printQ(readyq);
	printf("\n");
	printf("Blocked:\t");
	printQ(blockedq);
	printf("\n");
	printf("Create:\t");
	printQ(createq);
	printf("\n");
	while (1) {
		printf("请选择：\n");
		printf("\t2\tTimeout:\tRunning->Ready\n");
		printf("\t3\tEventWait:\tRunning->Blocked\n");
		printf("\t4\tEventOccurs:\tBlocked->Ready\n");
		printf("\t5\tCreate\n");
		printf("\t6\tExit\n");
		scanf("%d", &trans);
		switch (trans) {
			case 2:transfer2(runningq, readyq, blockedq, createq); break;
			case 3:transfer3(runningq, readyq, blockedq, createq); break;
			case 4:transfer4(runningq, readyq, blockedq, createq); break;
			case 5:process[pro_num] = transfer5(runningq, readyq, blockedq, createq, size_now); pro_num++; size_now = size_now + process[pro_num].mem_size; break;
			case 6:size_now=transfer6(runningq, readyq, blockedq, createq, size_now); break;
			default:printf("请输入正确的状态转换\n");
		}
	}
	return 0;
}
