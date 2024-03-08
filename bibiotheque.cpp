#include<stdio.h>
#include<string.h>
#include<time.h> 

#define LIMIT 2592000			//图书借阅时间最多为30天，超过时间则要进行罚款处理 

typedef struct book{
	int id;						//书籍id 
	char name[30];				//书名最长30个字符 
	int count;					//书籍剩余数量 
	int borrowReaderCount;		//有几个人借阅该书 
	int borrowReader[100];		//借阅该书的人的id（最多100人借阅一本书）
}BOOK;


typedef struct reader{
	int id;					//读者id 
	char name[30];			//读者名字 
	int hasBorrowed;		//是否有借阅书 
	char book[30];			//所借阅的书籍名称
	time_t time;			//借阅书籍时的时间 
}READER;


READER readers[100] = {'\0'};	//最多保存100位读者 
BOOK books[100] = {'\0'};		//最多保存100种图书 
int readersTotal = 0;			//读者总数 
int booksTotal = 0;				//图书总数 
time_t times = time(NULL);		//当前时间 


int initial()
{
	printf("---------------初始化---------------\n");
	FILE* in = fopen("books.txt", "r");
	if (in != NULL)
	{
		while (!feof(in))
		{
			fscanf(in,"%d %s %d %d", &books[booksTotal].id,&books[booksTotal].name, &books[booksTotal].count,&books[booksTotal].borrowReaderCount);
			if(books[booksTotal].borrowReaderCount>0)			//如果书籍有被借阅，则将借阅者的名字（可能多个）录入 
			{
				for(int i=0;i<books[booksTotal].borrowReaderCount;i++)
				{
					fscanf(in,"%d",&books[booksTotal].borrowReader[i]);
				}
			}
			booksTotal++;
		}
		printf("已加载图书列表文件！\n");
	}
	else
	{
		printf("图书列表文件丢失！\n");
		return 0;
	}
	in = fopen("readers.txt","r");
	if (in != NULL)
	{
		while (!feof(in))
		{
			fscanf(in,"%d %s %d", &readers[readersTotal].id,&readers[readersTotal].name, &readers[readersTotal].hasBorrowed);
			if(readers[readersTotal].hasBorrowed==1)			//如果读者有借阅书籍，则录入书名 
			{
				fscanf(in,"%s %d",&readers[readersTotal].book,&readers[readersTotal].time);
			}
			readersTotal++;
		}
		printf("已加载读者信息文件！\n");
	}
	else
	{
		printf("读者信息文件丢失！\n");
		return 0;
	}
	fclose(in);
	return 1;
}

void search(char* name)			//查询书籍 
{
	printf("--------------查询书籍--------------\n");
	int i, foundflag = 0;
	for(i=0;i<booksTotal;i++)
	{
		if (strcmp(books[i].name, name) == 0)
		{
			foundflag = 1;
			printf("%s 剩余%d本\n", books[i].name, books[i].count);
			if(books[i].borrowReaderCount>0)
			{
				printf("正在借阅本书的读者："); 			//还可看到借阅该书的读者的名称 
				for(int j=0;j<books[i].borrowReaderCount;j++)
				{
					printf(" %s ",readers[books[i].borrowReader[j]].name);
				}
				printf("\n");
			}
			else
			{
				printf("没有读者借阅该书\n");
			}
			break;
		}
	}
	if (foundflag == 0)
	{
		printf("找不到书籍！\n");
	}
}

void borrow(char* name,int id)	//借阅书籍 
{
	printf("--------------借阅书籍--------------\n");
	int i, foundflag = 0;
	for(i=0;i<booksTotal;i++)
	{
		if (strcmp(books[i].name, name) == 0)
		{
			foundflag = 1;
			if(books[i].count<1)
			{
				printf("该书馆存不足！\n");
				break;
			}
			printf("借阅了书籍！\n");
			books[i].count--;
			books[i].borrowReader[books[i].borrowReaderCount]=id;		//记录书籍借阅者的id到书籍数据中 
			books[i].borrowReaderCount++;
			readers[id].hasBorrowed=1;
			strcpy(readers[id].book,name);			//记录书籍名称到读者数据中 
			readers[id].time=time(NULL);			//记录读者借阅该书的时间 
			printf("%s 剩余%d本\n", books[i].name, books[i].count);
			break;
		}
	}
	if (foundflag == 0)
	{
		printf("找不到书籍！\n");
	}
}

void return_fun(int i,int j,int id)		//辅助函数（将归还的书籍已借阅的读者数据更新） 
{
	books[i].borrowReader[j]=0;
	for(int k=j;k<books[i].borrowReaderCount;k++)		//将移除的读者后的读者向前移，便于数组操作和保存 
	{
		int t=books[i].borrowReader[k+1];
		books[i].borrowReader[k+1]=books[i].borrowReader[k];
		books[i].borrowReader[k]=t;
	}
	books[i].borrowReaderCount--;
}

int impose(time_t t)		//超时罚款（按时间收取5/15/30元） ，未交则不能还书借书 
{
	printf("--------------超时罚款--------------\n");
	printf("借阅超过30天但未到60天的需缴纳5元\n");
	printf("借阅超过60天但未到120天的需缴纳15元\n");
	printf("借阅超过120天的需缴纳30元\n");
	int day = t/(24*60*60);
	printf("您借阅书籍的时间为 %d 天，需要缴纳罚款后方可正常借阅归还书籍\n",day);
	int c;
	if(t<=2*LIMIT)
	{
		c=5; 
	}
	else if(t>2*LIMIT&&t<=4*LIMIT)
	{
		c=15;
	}
	else
	{
		c=30;
	}
	printf("请缴纳 %d 元\n",c);
	printf("1：确认   2:取消\n");
	int in;
	while(1)
	{
		char in;
		scanf("%c", &in);
		scanf("%c", &in);
		switch (in)
		{
		case '1':
			printf("缴纳成功！\n");
			return 1;
		case '2':
			printf("已取消缴纳！\n");
			return 0;
		default:
			printf("选择不合法！请重新输入！\n");
		}
	}
}

void return_(char* name,int id)		//归还书籍 
{
	if(times-readers[id].time>LIMIT)
	{
		if(impose(times-readers[id].time)==0)
		{
			printf("归还书籍失败！\n");
			return;
		}
	}
	printf("--------------归还书籍--------------\n");
	int i, foundflag = 0;
	for(i=0;i<booksTotal;i++)
	{
		if (strcmp(books[i].name, name) == 0)
		{
			foundflag = 1;
			printf("归还了书籍 %s ！\n",name);
			books[i].count++;
			readers[id].hasBorrowed=0;
			char t[30]={'\0'};
			strcpy(readers[id].book,t);				//将编号为id的读者所借的图书改为空字符 
			for(int j=0;j<books[i].borrowReaderCount;j++)
			{
				if(books[i].borrowReader[j]==id)
				{
					return_fun(i,j,id);				//将相应书籍中“正在借阅中的读者”前移 
				}
			}
			printf("%s 剩余%d本\n", books[i].name, books[i].count);
			break;
		}
	}
	if (foundflag == 0)			//找不到相应书名则新增一个书名 
	{
		strcpy(books[booksTotal].name,name);
		books[booksTotal].count=1;
		booksTotal++;
		readers[id].hasBorrowed=0;
		printf("归还了书籍 %s ！\n",name);
		printf("%s 剩余%d本\n", books[i].name, books[i].count);
	}
}

void booksList()		//列出书籍列表 
{
	printf("--------------图书列表--------------\n");
	for(int i=0;i<booksTotal;i++)
	{
		printf("剩余%d本：     %s		",books[i].count,books[i].name);
		if(books[i].borrowReaderCount>0)
		{
			printf("正在借阅本书的读者："); 
			for(int j=0;j<books[i].borrowReaderCount;j++)
			{
				printf("	%s",readers[books[i].borrowReader[j]].name);
			}
		}
		else
		{
			printf("没有读者借阅该书");
		}
		printf("\n"); 
	}
}

void borrowInformation(int id)		//编号为id的读者的借阅情况 
{
	printf("------------书籍借阅情况------------\n");
	printf("读者 %s\n",readers[id].name);
	if(readers[id].hasBorrowed==0)
	{
		printf("您没有正在借阅中的书籍！\n");
	}
	else
	{
		printf("您正在借阅书籍 %s，已借阅 %d 天\n",readers[id].book,(times-readers[id].time)/(3600*24));
	}
}

void readersList()		//列出读者列表 
{
	printf("--------------读者列表--------------\n");
	for(int i=0;i<readersTotal;i++)
	{
		printf("id：%d     %s     ",readers[i].id,readers[i].name);
		if(readers[i].hasBorrowed==0)
		{
			printf("没有正在借阅的书籍\n");
		}
		else
		{
			printf("正在借阅  %s\n",readers[i].book);
		}
	}
}

void addBook()		//添加书籍 （已有则添加，没有则新建） 
{
	printf("--------------添加书籍--------------\n");
	printf("请输入您所要添加的书籍名称：\n");
	char name[30];
	scanf("%s",&name);
	printf("请输入您所要添加的书籍数量：\n");
	int count;
	scanf("%d",&count);
	int i, foundflag = 0;
	for(i=0;i<booksTotal;i++)
	{
		if (strcmp(books[i].name, name) == 0 && books[i].count>0)
		{
			foundflag = 1;
			printf("添加了书籍 %s ！\n",name);
			books[i].count+=count;
			printf("%s 馆存%d本\n", books[i].name, books[i].count);
			break;
		}
	}
	if (foundflag == 0)
	{
		strcpy(books[booksTotal].name,name);
		books[booksTotal].count=count;
		booksTotal++;
		printf("添加了书籍 %s ！\n",name);
		printf("%s 馆存%d本\n", books[i].name, books[i].count);
	}
}

void removeBook_fun(int id)		//辅助函数（移除后，后面的书籍前移）
{
	books[id].id=0;
	for(int i=id;i<booksTotal;i++)
	{
		BOOK bo = books[i];
		books[i] = books[i+1];
		books[i+1] = bo;
		books[i].id=i;
	}
	booksTotal--;
} 

void removeBook()		//移除书籍（此处书籍个数归为0不删除，便于图书记录） 
{
	printf("--------------移除书籍--------------\n");
	printf("请输入您所要移除的书籍名称：\n");
	char name[30];
	scanf("%s",&name);
	printf("请输入您所要移除的书籍数量：\n");
	int count;
	scanf("%d",&count);
	int i, foundflag = 0;
	for(i=0;i<booksTotal;i++)
	{
		if (strcmp(books[i].name, name) == 0)
		{
			foundflag = 1;
			if(books[i].count<count)
			{
				printf("移除的书籍超过剩余数量！\n");
				break;
			}
			printf("移除了书籍 %s ！\n",name);
			books[i].count-=count;
			if(books[i].count==0&&books[i].borrowReaderCount==0)
			{
				removeBook_fun(i);
				break;
			}
			printf("%s 馆存%d本\n", books[i].name, books[i].count);
			break;
		}
	}
	if (foundflag == 0)
	{
		printf("找不到书籍！\n");
	}
}

void adminBook()		//管理图书界面 
{
	while(1)
	{
		printf("---------管理员图书管理列表---------\n");
		printf("请输入相应的数字进行操作：\n");          
		printf("1：图书列表   ");
		printf("2：添加图书   ");
		printf("3：移除图书   ");
		printf("4：返回上级\n");
		char in;
		scanf("%c", &in);
		scanf("%c", &in);
		switch (in)
		{
		case '1':
			booksList();
			break;
		case '2':
			addBook(); 
			break;
		case '3':
			removeBook();
			break;
		case '4':
			return;
		default:
			printf("选择不合法！请重新输入！\n");
		}
	}
}

void addReader()		//添加读者 
{
	printf("--------------添加读者--------------\n");
	printf("请输入您所要添加的读者名称：\n");
	char name[30];
	scanf("%s",&name);
	int i, foundflag = 0;
	for(i=0;i<readersTotal;i++)
	{
		if (strcmp(readers[i].name, name) == 0)
		{
			foundflag = 1;
			printf("该读者已存在！无法再次添加！\n");
			break;
		}
	}
	if (foundflag == 0)
	{
		strcpy(readers[readersTotal].name,name);
		readers[readersTotal].id=readersTotal;
		readers[readersTotal].hasBorrowed=0;
		readersTotal++;
		printf("添加了读者 %s ！\n",name);
		printf("id号为%d\n", readersTotal-1);
	}
}

void removeReader_fun(int id)		//辅助函数（移除后，后面的读者前移） 
{
	readers[id].id=0;
	readers[id].hasBorrowed=0;
	for(int i=id;i<readersTotal;i++)
	{
		READER re = readers[i];
		readers[i] = readers[i+1];
		readers[i+1] = re;
		readers[i].id=i;
	}
	readersTotal--;
}

void removeReader()		//移除读者 
{
	printf("--------------移除读者--------------\n");
	printf("请输入您所要移除的读者名称：\n");
	char name[30];
	scanf("%s",&name);
	int i, foundflag = 0;
	for(i=0;i<readersTotal;i++)
	{
		if (strcmp(readers[i].name, name) == 0)
		{
			foundflag = 1;
			if(readers[i].hasBorrowed==1)
			{
				printf("该读者还有书籍未归还！无法移除！\n");
				break;
			}
			removeReader_fun(i);
			printf("读者 %s 已移除！\n",name);
			break;
		}
	}
	if (foundflag == 0)
	{
		printf("找不到读者！移除失败！\n");
	}
}

void adminReader()		//管理读者界面 
{
	while(1)
	{
		printf("---------管理员读者管理列表---------\n");
		printf("请输入相应的数字进行操作：\n"); 
		loop6:           
		printf("1：读者列表   ");
		printf("2：添加读者   ");
		printf("3：移除读者   ");
		printf("4：返回上级\n");
		char in;
		scanf("%c", &in);
		scanf("%c", &in);
		switch (in)
		{
		case '1':
			readersList();
			break;
		case '2':
			addReader(); 
			break;
		case '3':
			removeReader();
			break;
		case '4':
			return;
		default:
			printf("选择不合法！请重新输入！\n");
		}
	}
}

void admin()		//管理员界面 
{
	while(1)
	{
		printf("-----------管理员操作列表-----------\n");
		printf("您的身份为管理员，请输入相应的数字进行操作：\n"); 
		loop4:           
		printf("1：管理图书   ");
		printf("2：管理读者   ");
		printf("3：返回上级\n");
		char in;
		scanf("%c", &in);
		scanf("%c", &in);
		switch (in)
		{
		case '1':
			adminBook();
			break;
		case '2':
			adminReader();
			break;
		case '3':
			return;
		default:
			printf("选择不合法！请重新输入！\n");
		}
	}
}

void reader()		//读者界面 
{
	printf("您的身份为读者，请输入您的id：\n");
	int readerId;
	loop2:
	scanf("%d",&readerId);
	if(!(readerId>=0&&readerId<readersTotal))
	{
		printf("找不到相应id！请重新输入！\n");
		goto loop2;
	}
	while(1)
	{
		printf("------------读者操作界面------------\n");
		printf("欢迎读者 %s ，请输入相应的数字进行操作：\n",readers[readerId].name); 
		loop3:           
		printf("1：查询书籍   ");
		printf("2：借阅书籍   ");
		printf("3：归还当前借阅中的书籍   ");
		printf("4：图书列表   ");
		printf("5：本id书籍借阅情况   ");
		printf("6：返回上级\n");
		char in;
		scanf("%c", &in);
		scanf("%c", &in);
		char name[20];
		switch (in)
		{
		case '1':
			printf("请输入所查询的书籍名称：");
			scanf("\n%s", &name);
			search(name);
			break;
		case '2':
			if(readers[readerId].hasBorrowed==0)
			{
				printf("请输入所借阅的书籍名称：");
				scanf("\n%s", &name);
				borrow(name,readerId);
			}
			else
			{
				printf("您已经有正在借阅的书籍！不可借阅！\n");
			}
			break;
		case '3':
			if(readers[readerId].hasBorrowed==1)
			{
				return_(readers[readerId].book,readerId);
			}
			else
			{
				printf("您目前没有正在借阅的书籍！\n");
			}
			break;
		case '4':
			booksList();
			break;
		case '5':
			borrowInformation(readerId);
			break;
		case '6':
			return;
		default:
			printf("选择不合法！请重新输入！\n");
		}
	} 
}

void exit()			//保存数据到文件 
{
	printf("---------------保存中---------------\n");
	FILE* out = fopen("books.txt", "w");
	if (out != NULL)
	{
		for(int i=0;i<booksTotal;i++)
		{
			fprintf(out,"%d %s %d %d", books[i].id,books[i].name, books[i].count,books[i].borrowReaderCount);
			if(books[i].borrowReaderCount>0)
			{
				for(int j=0;j<books[i].borrowReaderCount;j++)
				{
					fprintf(out," %d",books[i].borrowReader[j]);
				}
			}
			if(i<booksTotal-1)
			{
				fprintf(out,"\n");
			}
		}
		printf("已保存图书列表文件！\n");
	}
	else
	{
		printf("图书列表文件丢失！\n");
		return;
	}
	out = fopen("readers.txt","w");
	if (out != NULL)
	{
		for(int i=0;i<readersTotal;i++)
		{
			fprintf(out,"%d %s %d", readers[i].id,readers[i].name, readers[i].hasBorrowed);
			if(readers[i].hasBorrowed==1)
			{
				fprintf(out," %s %d",readers[i].book,readers[i].time);
			} 
			if(i<readersTotal-1)
			{
				fprintf(out,"\n");
			}
		}
		printf("已保存读者信息文件！\n");
	}
	else
	{
		printf("读者信息文件丢失！\n");
		return;
	}
	fclose(out);
}

int main()
{
	if(initial()==0)
	{
		return 0;
	}
	while (1)
	{
		printf("---------------主界面---------------\n");
		printf("欢迎来到图书馆管理系统！\n");
		printf("请输入“1”或“2”选择您的身份：\n");
		loop:
		printf("1：读者\n");
		printf("2：管理员\n");
		printf("3：保存数据并退出\n");
		char in;
		scanf("%c", &in);
		switch (in)
		{
		case '1':
			reader();
			break;
		case '2':
			admin();
			break;
		case '3':
			exit();
			printf("谢谢使用！");
			return 0;
		default:
			printf("选择不合法！请重新输入！\n");
		}
	}
	return 0;
}
