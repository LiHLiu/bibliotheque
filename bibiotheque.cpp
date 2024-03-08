#include<stdio.h>
#include<string.h>
#include<time.h> 

#define LIMIT 2592000			//ͼ�����ʱ�����Ϊ30�죬����ʱ����Ҫ���з���� 

typedef struct book{
	int id;						//�鼮id 
	char name[30];				//�����30���ַ� 
	int count;					//�鼮ʣ������ 
	int borrowReaderCount;		//�м����˽��ĸ��� 
	int borrowReader[100];		//���ĸ�����˵�id�����100�˽���һ���飩
}BOOK;


typedef struct reader{
	int id;					//����id 
	char name[30];			//�������� 
	int hasBorrowed;		//�Ƿ��н����� 
	char book[30];			//�����ĵ��鼮����
	time_t time;			//�����鼮ʱ��ʱ�� 
}READER;


READER readers[100] = {'\0'};	//��ౣ��100λ���� 
BOOK books[100] = {'\0'};		//��ౣ��100��ͼ�� 
int readersTotal = 0;			//�������� 
int booksTotal = 0;				//ͼ������ 
time_t times = time(NULL);		//��ǰʱ�� 


int initial()
{
	printf("---------------��ʼ��---------------\n");
	FILE* in = fopen("books.txt", "r");
	if (in != NULL)
	{
		while (!feof(in))
		{
			fscanf(in,"%d %s %d %d", &books[booksTotal].id,&books[booksTotal].name, &books[booksTotal].count,&books[booksTotal].borrowReaderCount);
			if(books[booksTotal].borrowReaderCount>0)			//����鼮�б����ģ��򽫽����ߵ����֣����ܶ����¼�� 
			{
				for(int i=0;i<books[booksTotal].borrowReaderCount;i++)
				{
					fscanf(in,"%d",&books[booksTotal].borrowReader[i]);
				}
			}
			booksTotal++;
		}
		printf("�Ѽ���ͼ���б��ļ���\n");
	}
	else
	{
		printf("ͼ���б��ļ���ʧ��\n");
		return 0;
	}
	in = fopen("readers.txt","r");
	if (in != NULL)
	{
		while (!feof(in))
		{
			fscanf(in,"%d %s %d", &readers[readersTotal].id,&readers[readersTotal].name, &readers[readersTotal].hasBorrowed);
			if(readers[readersTotal].hasBorrowed==1)			//��������н����鼮����¼������ 
			{
				fscanf(in,"%s %d",&readers[readersTotal].book,&readers[readersTotal].time);
			}
			readersTotal++;
		}
		printf("�Ѽ��ض�����Ϣ�ļ���\n");
	}
	else
	{
		printf("������Ϣ�ļ���ʧ��\n");
		return 0;
	}
	fclose(in);
	return 1;
}

void search(char* name)			//��ѯ�鼮 
{
	printf("--------------��ѯ�鼮--------------\n");
	int i, foundflag = 0;
	for(i=0;i<booksTotal;i++)
	{
		if (strcmp(books[i].name, name) == 0)
		{
			foundflag = 1;
			printf("%s ʣ��%d��\n", books[i].name, books[i].count);
			if(books[i].borrowReaderCount>0)
			{
				printf("���ڽ��ı���Ķ��ߣ�"); 			//���ɿ������ĸ���Ķ��ߵ����� 
				for(int j=0;j<books[i].borrowReaderCount;j++)
				{
					printf(" %s ",readers[books[i].borrowReader[j]].name);
				}
				printf("\n");
			}
			else
			{
				printf("û�ж��߽��ĸ���\n");
			}
			break;
		}
	}
	if (foundflag == 0)
	{
		printf("�Ҳ����鼮��\n");
	}
}

void borrow(char* name,int id)	//�����鼮 
{
	printf("--------------�����鼮--------------\n");
	int i, foundflag = 0;
	for(i=0;i<booksTotal;i++)
	{
		if (strcmp(books[i].name, name) == 0)
		{
			foundflag = 1;
			if(books[i].count<1)
			{
				printf("����ݴ治�㣡\n");
				break;
			}
			printf("�������鼮��\n");
			books[i].count--;
			books[i].borrowReader[books[i].borrowReaderCount]=id;		//��¼�鼮�����ߵ�id���鼮������ 
			books[i].borrowReaderCount++;
			readers[id].hasBorrowed=1;
			strcpy(readers[id].book,name);			//��¼�鼮���Ƶ����������� 
			readers[id].time=time(NULL);			//��¼���߽��ĸ����ʱ�� 
			printf("%s ʣ��%d��\n", books[i].name, books[i].count);
			break;
		}
	}
	if (foundflag == 0)
	{
		printf("�Ҳ����鼮��\n");
	}
}

void return_fun(int i,int j,int id)		//�������������黹���鼮�ѽ��ĵĶ������ݸ��£� 
{
	books[i].borrowReader[j]=0;
	for(int k=j;k<books[i].borrowReaderCount;k++)		//���Ƴ��Ķ��ߺ�Ķ�����ǰ�ƣ�������������ͱ��� 
	{
		int t=books[i].borrowReader[k+1];
		books[i].borrowReader[k+1]=books[i].borrowReader[k];
		books[i].borrowReader[k]=t;
	}
	books[i].borrowReaderCount--;
}

int impose(time_t t)		//��ʱ�����ʱ����ȡ5/15/30Ԫ�� ��δ�����ܻ������ 
{
	printf("--------------��ʱ����--------------\n");
	printf("���ĳ���30�쵫δ��60��������5Ԫ\n");
	printf("���ĳ���60�쵫δ��120��������15Ԫ\n");
	printf("���ĳ���120��������30Ԫ\n");
	int day = t/(24*60*60);
	printf("�������鼮��ʱ��Ϊ %d �죬��Ҫ���ɷ���󷽿��������Ĺ黹�鼮\n",day);
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
	printf("����� %d Ԫ\n",c);
	printf("1��ȷ��   2:ȡ��\n");
	int in;
	while(1)
	{
		char in;
		scanf("%c", &in);
		scanf("%c", &in);
		switch (in)
		{
		case '1':
			printf("���ɳɹ���\n");
			return 1;
		case '2':
			printf("��ȡ�����ɣ�\n");
			return 0;
		default:
			printf("ѡ�񲻺Ϸ������������룡\n");
		}
	}
}

void return_(char* name,int id)		//�黹�鼮 
{
	if(times-readers[id].time>LIMIT)
	{
		if(impose(times-readers[id].time)==0)
		{
			printf("�黹�鼮ʧ�ܣ�\n");
			return;
		}
	}
	printf("--------------�黹�鼮--------------\n");
	int i, foundflag = 0;
	for(i=0;i<booksTotal;i++)
	{
		if (strcmp(books[i].name, name) == 0)
		{
			foundflag = 1;
			printf("�黹���鼮 %s ��\n",name);
			books[i].count++;
			readers[id].hasBorrowed=0;
			char t[30]={'\0'};
			strcpy(readers[id].book,t);				//�����Ϊid�Ķ��������ͼ���Ϊ���ַ� 
			for(int j=0;j<books[i].borrowReaderCount;j++)
			{
				if(books[i].borrowReader[j]==id)
				{
					return_fun(i,j,id);				//����Ӧ�鼮�С����ڽ����еĶ��ߡ�ǰ�� 
				}
			}
			printf("%s ʣ��%d��\n", books[i].name, books[i].count);
			break;
		}
	}
	if (foundflag == 0)			//�Ҳ�����Ӧ����������һ������ 
	{
		strcpy(books[booksTotal].name,name);
		books[booksTotal].count=1;
		booksTotal++;
		readers[id].hasBorrowed=0;
		printf("�黹���鼮 %s ��\n",name);
		printf("%s ʣ��%d��\n", books[i].name, books[i].count);
	}
}

void booksList()		//�г��鼮�б� 
{
	printf("--------------ͼ���б�--------------\n");
	for(int i=0;i<booksTotal;i++)
	{
		printf("ʣ��%d����     %s		",books[i].count,books[i].name);
		if(books[i].borrowReaderCount>0)
		{
			printf("���ڽ��ı���Ķ��ߣ�"); 
			for(int j=0;j<books[i].borrowReaderCount;j++)
			{
				printf("	%s",readers[books[i].borrowReader[j]].name);
			}
		}
		else
		{
			printf("û�ж��߽��ĸ���");
		}
		printf("\n"); 
	}
}

void borrowInformation(int id)		//���Ϊid�Ķ��ߵĽ������ 
{
	printf("------------�鼮�������------------\n");
	printf("���� %s\n",readers[id].name);
	if(readers[id].hasBorrowed==0)
	{
		printf("��û�����ڽ����е��鼮��\n");
	}
	else
	{
		printf("�����ڽ����鼮 %s���ѽ��� %d ��\n",readers[id].book,(times-readers[id].time)/(3600*24));
	}
}

void readersList()		//�г������б� 
{
	printf("--------------�����б�--------------\n");
	for(int i=0;i<readersTotal;i++)
	{
		printf("id��%d     %s     ",readers[i].id,readers[i].name);
		if(readers[i].hasBorrowed==0)
		{
			printf("û�����ڽ��ĵ��鼮\n");
		}
		else
		{
			printf("���ڽ���  %s\n",readers[i].book);
		}
	}
}

void addBook()		//����鼮 ����������ӣ�û�����½��� 
{
	printf("--------------����鼮--------------\n");
	printf("����������Ҫ��ӵ��鼮���ƣ�\n");
	char name[30];
	scanf("%s",&name);
	printf("����������Ҫ��ӵ��鼮������\n");
	int count;
	scanf("%d",&count);
	int i, foundflag = 0;
	for(i=0;i<booksTotal;i++)
	{
		if (strcmp(books[i].name, name) == 0 && books[i].count>0)
		{
			foundflag = 1;
			printf("������鼮 %s ��\n",name);
			books[i].count+=count;
			printf("%s �ݴ�%d��\n", books[i].name, books[i].count);
			break;
		}
	}
	if (foundflag == 0)
	{
		strcpy(books[booksTotal].name,name);
		books[booksTotal].count=count;
		booksTotal++;
		printf("������鼮 %s ��\n",name);
		printf("%s �ݴ�%d��\n", books[i].name, books[i].count);
	}
}

void removeBook_fun(int id)		//�����������Ƴ��󣬺�����鼮ǰ�ƣ�
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

void removeBook()		//�Ƴ��鼮���˴��鼮������Ϊ0��ɾ��������ͼ���¼�� 
{
	printf("--------------�Ƴ��鼮--------------\n");
	printf("����������Ҫ�Ƴ����鼮���ƣ�\n");
	char name[30];
	scanf("%s",&name);
	printf("����������Ҫ�Ƴ����鼮������\n");
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
				printf("�Ƴ����鼮����ʣ��������\n");
				break;
			}
			printf("�Ƴ����鼮 %s ��\n",name);
			books[i].count-=count;
			if(books[i].count==0&&books[i].borrowReaderCount==0)
			{
				removeBook_fun(i);
				break;
			}
			printf("%s �ݴ�%d��\n", books[i].name, books[i].count);
			break;
		}
	}
	if (foundflag == 0)
	{
		printf("�Ҳ����鼮��\n");
	}
}

void adminBook()		//����ͼ����� 
{
	while(1)
	{
		printf("---------����Աͼ������б�---------\n");
		printf("��������Ӧ�����ֽ��в�����\n");          
		printf("1��ͼ���б�   ");
		printf("2�����ͼ��   ");
		printf("3���Ƴ�ͼ��   ");
		printf("4�������ϼ�\n");
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
			printf("ѡ�񲻺Ϸ������������룡\n");
		}
	}
}

void addReader()		//��Ӷ��� 
{
	printf("--------------��Ӷ���--------------\n");
	printf("����������Ҫ��ӵĶ������ƣ�\n");
	char name[30];
	scanf("%s",&name);
	int i, foundflag = 0;
	for(i=0;i<readersTotal;i++)
	{
		if (strcmp(readers[i].name, name) == 0)
		{
			foundflag = 1;
			printf("�ö����Ѵ��ڣ��޷��ٴ���ӣ�\n");
			break;
		}
	}
	if (foundflag == 0)
	{
		strcpy(readers[readersTotal].name,name);
		readers[readersTotal].id=readersTotal;
		readers[readersTotal].hasBorrowed=0;
		readersTotal++;
		printf("����˶��� %s ��\n",name);
		printf("id��Ϊ%d\n", readersTotal-1);
	}
}

void removeReader_fun(int id)		//�����������Ƴ��󣬺���Ķ���ǰ�ƣ� 
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

void removeReader()		//�Ƴ����� 
{
	printf("--------------�Ƴ�����--------------\n");
	printf("����������Ҫ�Ƴ��Ķ������ƣ�\n");
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
				printf("�ö��߻����鼮δ�黹���޷��Ƴ���\n");
				break;
			}
			removeReader_fun(i);
			printf("���� %s ���Ƴ���\n",name);
			break;
		}
	}
	if (foundflag == 0)
	{
		printf("�Ҳ������ߣ��Ƴ�ʧ�ܣ�\n");
	}
}

void adminReader()		//������߽��� 
{
	while(1)
	{
		printf("---------����Ա���߹����б�---------\n");
		printf("��������Ӧ�����ֽ��в�����\n"); 
		loop6:           
		printf("1�������б�   ");
		printf("2����Ӷ���   ");
		printf("3���Ƴ�����   ");
		printf("4�������ϼ�\n");
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
			printf("ѡ�񲻺Ϸ������������룡\n");
		}
	}
}

void admin()		//����Ա���� 
{
	while(1)
	{
		printf("-----------����Ա�����б�-----------\n");
		printf("�������Ϊ����Ա����������Ӧ�����ֽ��в�����\n"); 
		loop4:           
		printf("1������ͼ��   ");
		printf("2���������   ");
		printf("3�������ϼ�\n");
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
			printf("ѡ�񲻺Ϸ������������룡\n");
		}
	}
}

void reader()		//���߽��� 
{
	printf("�������Ϊ���ߣ�����������id��\n");
	int readerId;
	loop2:
	scanf("%d",&readerId);
	if(!(readerId>=0&&readerId<readersTotal))
	{
		printf("�Ҳ�����Ӧid�����������룡\n");
		goto loop2;
	}
	while(1)
	{
		printf("------------���߲�������------------\n");
		printf("��ӭ���� %s ����������Ӧ�����ֽ��в�����\n",readers[readerId].name); 
		loop3:           
		printf("1����ѯ�鼮   ");
		printf("2�������鼮   ");
		printf("3���黹��ǰ�����е��鼮   ");
		printf("4��ͼ���б�   ");
		printf("5����id�鼮�������   ");
		printf("6�������ϼ�\n");
		char in;
		scanf("%c", &in);
		scanf("%c", &in);
		char name[20];
		switch (in)
		{
		case '1':
			printf("����������ѯ���鼮���ƣ�");
			scanf("\n%s", &name);
			search(name);
			break;
		case '2':
			if(readers[readerId].hasBorrowed==0)
			{
				printf("�����������ĵ��鼮���ƣ�");
				scanf("\n%s", &name);
				borrow(name,readerId);
			}
			else
			{
				printf("���Ѿ������ڽ��ĵ��鼮�����ɽ��ģ�\n");
			}
			break;
		case '3':
			if(readers[readerId].hasBorrowed==1)
			{
				return_(readers[readerId].book,readerId);
			}
			else
			{
				printf("��Ŀǰû�����ڽ��ĵ��鼮��\n");
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
			printf("ѡ�񲻺Ϸ������������룡\n");
		}
	} 
}

void exit()			//�������ݵ��ļ� 
{
	printf("---------------������---------------\n");
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
		printf("�ѱ���ͼ���б��ļ���\n");
	}
	else
	{
		printf("ͼ���б��ļ���ʧ��\n");
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
		printf("�ѱ��������Ϣ�ļ���\n");
	}
	else
	{
		printf("������Ϣ�ļ���ʧ��\n");
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
		printf("---------------������---------------\n");
		printf("��ӭ����ͼ��ݹ���ϵͳ��\n");
		printf("�����롰1����2��ѡ��������ݣ�\n");
		loop:
		printf("1������\n");
		printf("2������Ա\n");
		printf("3���������ݲ��˳�\n");
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
			printf("ллʹ�ã�");
			return 0;
		default:
			printf("ѡ�񲻺Ϸ������������룡\n");
		}
	}
	return 0;
}
