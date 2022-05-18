#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void clear() {
#ifdef __unix__  /* __unix__ is usually defined by compilers targeting Unix systems */
	system("clear");

#elif defined(_WIN32) || defined(WIN32)  /* _Win32 is usually defined by compilers targeting 32 or   64 bit Windows systems */
	system("cls");
#endif
}

#define FOR(i, a, b, s) for (int i=(a); (s)>0?i<(b):i>(b); i+=(s))
#define FOR1(e) FOR(i, 0, e, 1)
#define FOR2(i, e) FOR(i, 0, e, 1)

enum identification{CIN,CNE,APOGEE};
typedef enum identification identification;


enum Type_Utilisateure{ETUDIANT,ADMINISTRATEUR};
typedef enum Type_Utilisateure Type_Utilisateure;

union Nom_Utilisateur{
    char CIN[8];
    char CNE[10] ;
    int APOGEE ;
};
typedef union Nom_Utilisateur Nom_Utilisateur;


struct User{
    identification id;
    Nom_Utilisateur login;
    char password[20];
    Type_Utilisateure type;
    char nom[20];
    char prenom[20];
    char date_N[10];
    struct User* suivant;
};
typedef struct User User;

struct Livre{
    char ISBN[13];
    char title[50];
    char auteur[50];
    struct Livre* suivant;
};
typedef struct Livre Livre;


typedef Livre* BLIST;
typedef User* ULIST;
typedef char* string;
typedef unsigned int u_int;







int isCap(char msg){
	if(msg >= 'a' && msg <= 'z'){
		return 0;
	}else if(msg >= 'A' && msg <= 'Z'){
		return 1;
	}
	return 2;
}


char* crypt(char* message, int key){
	char* msg = (char*)malloc(strlen(message)*sizeof(char));
	strcpy(msg, message);
	int i=0;
	while(msg[i] != '\0'){
		if( isCap(msg[i]) == 0){
			msg[i] = ((msg[i]+key) - 'a')%26+'a';
			//if((msg[i] > 'z' && isCap(msg[i]-5) == 0)  || msg[i] > 'Z' && isCap(msg[i]-5) == 1){
			//	msg[i] -= 26;
			//}
		}
		else if(isCap(msg[i]) == 1){
			 msg[i] = ((msg[i]+key) - 'A')%26+'A';
		}
		++i;
	}
	return msg;
}

char* decrypt(char* message, int key){
	char* msg = (char*)malloc(strlen(message)*sizeof(char));
	strcpy(msg, message);
	int i=0;
	while(msg[i] != '\0'){
		if( isCap(msg[i]) == 0){
			msg[i] = (msg[i]-key) ;
			if(msg[i] - 'a'< 0){
				 msg[i] += 26;
			 }
		}
		else if(isCap(msg[i]) == 1){
			 msg[i] = (msg[i]-key);
			 if(msg[i]  - 'A'< 0){
				 msg[i] += 26;
			 }
		}
		++i;
	}
	return msg;
}




Livre* makeBook(
    char ISBN[13],
    char title[50],
    char auteur[50]
)
{
    Livre* lv = (Livre*) malloc(sizeof(Livre));
    strcpy(lv->auteur,auteur);
    strcpy(lv->ISBN,ISBN);
    strcpy(lv->title,title);
    lv->suivant = NULL;
    return lv;
}

void add_no_read(BLIST* lv,Livre* livre){
    livre->suivant = *lv;
    *lv = livre;
}


User* makeUser(
    identification id,
    Nom_Utilisateur login,
    char password[20],
    Type_Utilisateure type,
    char nom[20],
    char prenom[20],
    char date_N[10]
)
{
    User* new_user = (User*)malloc(sizeof(User));
    new_user->id = id;
    new_user->login = login;
    strcpy(new_user->nom, nom);
    strcpy(new_user->prenom, prenom);
    strcpy(new_user->password, password);
    strcpy(new_user->date_N, date_N);
    new_user->type = type;
    new_user->suivant = NULL;
    return new_user;
}






User* Read_User()
{
    // get the id
    User* nuser = (User*)malloc(sizeof(User));
    char option;
    int exit = 0;

    do
    {
        clear();
        printf("|------------------------------------|\n");
        printf("|             Welcome!               |\n");
        printf("|                                    |\n");
        printf("|Select one of the following options:|\n");
        printf("|    1 - CIN                         |\n");
        printf("|    2 - CNE                         |\n");
        printf("|    3 - APOGEE                      |\n");
        printf("|    4 - Exit                        |\n");
        printf("|------------------------------------|\n");

        scanf(" %c", &option);

        switch(option)
        {
            case '1':
                nuser->id = CIN;
                printf("CIN : \n"); scanf("%s", nuser->login.CIN);
                exit = 1;
                break;

            case '2':
                nuser->id = CNE;
                printf("CNE : \n"); scanf("%s", nuser->login.CNE);
                exit = 1;
                break;

            case '3':
                nuser->id = APOGEE;
                printf("APOGEE : \n"); scanf("%d", &nuser->login.APOGEE);
                exit = 1;
                break;

            default:
                printf("Invalid option!\n\n");
                break;
        }
    }while(exit == 0);
    printf("nom : \n"); scanf("%s",nuser->nom);
    printf("prenom : \n"); scanf("%s",nuser->prenom);
    char pass[20];
    printf("password : \n"); scanf("%s",pass);
    strcpy(nuser->password,crypt(pass,10));
    printf("date jj/mm/yyyy : \n"); scanf("%s",nuser->date_N);
    char type[10];
    printf("Account type ADMIN , ETUDIANT\n"); scanf("%s",type);
    if(!strcmp(type,"ADMIN")){
        nuser->type = ADMINISTRATEUR;
    } else{
        nuser->type = ETUDIANT;
    }
    nuser->suivant = NULL;
    return nuser;
}

Livre* Read_Book(){
    Livre* lv = (Livre*) malloc(sizeof(Livre));
    printf("ISBN : \n"); scanf("%s",lv->ISBN);
    printf("title : \n"); scanf("%s",lv->title);
    printf("auteur : \n"); scanf("%s",lv->auteur);
    lv->suivant = NULL;
    return lv;
}


void save_User(char path[200],User* u){
    FILE* fptr = fopen(path,"ab");
    if(!fptr){
        fprintf(stderr,"ERROR : cannot load the books !! \n");
        return;
    }
    fwrite((void*) u,sizeof(User),1,fptr);
    fclose(fptr);
}

void Add_User(ULIST* USER_LIST){
    User* new_user = Read_User();
    save_User("user.bin",new_user);
    new_user->suivant = *USER_LIST;
    *USER_LIST = new_user;
}


void save_book(char path[200],Livre* lv){
    FILE* fptr = fopen(path,"a");
    if(!fptr){
        fprintf(stderr,"ERROR : cannot load the books !! \n");
        return;
    }

    fprintf(fptr,"\n%s",lv->ISBN);
    fprintf(fptr,"\n%s",lv->title);
    fprintf(fptr,"\n%s\n",lv->auteur);

    fclose(fptr);
}


void Add_Book(BLIST* books){
    Livre* new_book = Read_Book();
    save_book("books.lv",new_book);
    new_book->suivant = *books;
    *books = new_book;

}

void user_info(User* user){
    printf("===================================================================\n");
        printf("nom : %s\n",user->nom);
        printf("prenom : %s\n",user->prenom);
        printf("date of bith : %s\n",user->date_N);
        switch(user->id)
        {
            case CIN:
                printf("CIN : %s\n",user->login.CIN);
                break;

            case CNE:
                printf("CNE : %s\n",user->login.CIN);
                break;

            case APOGEE:
                printf("CNE : %d\n",user->login.APOGEE);
                break;

            default:
                printf("Invalid option!\n\n");
                break;
        }
        printf("passwrd : %s\n",user->password);
        if(user->type == ADMINISTRATEUR){
            printf("Account type is ADMIN \n");
        } else{ 
            printf("Account type is ETUDIENT \n");
        }
        printf("===================================================================\n");

}

void book_info(Livre* lv){
    printf("===================================================================\n");
        printf("ISBN : %s\n",lv->ISBN);
        printf("title : %s\n",lv->title);
        printf("auteur : %s\n",lv->auteur);
        printf("===================================================================\n");
}



void Display_Users(ULIST* ls){
    clear();
    User* temp = *ls;
    while(temp != NULL){
        user_info(temp);
        temp = temp->suivant;
    }
    printf("Press any key to go back .... \n");
    fgetc(stdin);
    fgetc(stdin);
}

void Display_Books(BLIST* BOOKS){
    clear();
    Livre* temp = *BOOKS;
    while(temp != NULL){
        book_info(temp);
        temp = temp->suivant;
    }
    printf("Press any key to go back .... \n");
    fgetc(stdin);
    fgetc(stdin);
}


User* autho(ULIST* ls, identification id, string loggin, string pass){
    User* temp = *ls;
    char p[20];
    while (temp != NULL)
    {
        strcpy(p,decrypt(temp->password,10));
        switch(id)
        {
            case CIN:
                if(!strcmp(loggin,temp->login.CIN) && temp->id == id)
                    if(!strcmp(pass,p))
                        return temp;
                break;

            case CNE:
                if(!strcmp(loggin,temp->login.CNE) && temp->id == id)
                    if(!strcmp(pass,p))
                        return temp;
                break;

            case APOGEE:
                if(temp->login.APOGEE == atoi(loggin) && temp->id == id )
                    if(!strcmp(pass,p))
                        return temp;
                break;

            default:
                printf("Invalid id\n\n");
                break;
        }
       temp = temp->suivant;
    }
    return NULL;
}
void add_no_read_user(ULIST* users,User* u){
    u->suivant = *users;
    *users = u;
}

void load_User(ULIST* users, char path[200]){
    FILE* fptr = fopen(path,"rb");
    if(!fptr){
        fprintf(stderr,"ERROR : cannot load the books !! \n");
        return;
    }
    User* user;
    while (1){
        if(feof(fptr)) break;
        user = (User*) malloc(sizeof(User));
        fread(user,sizeof(User),1,fptr);
        add_no_read_user(users,user);
    }
    fclose(fptr);
}



void load_Books(BLIST* lv, char path[200]){
    FILE* fptr = fopen(path,"r");
    if(!fptr){
        fprintf(stderr,"ERROR : cannot load the books !! \n");
        return;
    }
    char ISBN[13];
    char title[50];
    char auteur[50];
    Livre* liv;
    char line[50];
    int flag = 0;
    start:
    while (fgets(line,sizeof(line),fptr))
    {
        if(strlen(line) == 0){
            goto start;
        }
        if(flag == 0){
            strcpy(ISBN,line);
            flag = 1;
        }else if(flag == 1){
            strcpy(title,line);
            flag = 2; 
        }else if(flag == 2){
            strcpy(auteur,line);
            liv = makeBook(ISBN,title,auteur);
            add_no_read(lv,liv);
            flag = 0; 
        }
    }
    fclose(fptr);
}


void Menu_Admin(ULIST* ls,BLIST* lv){
    char option;
    int exit = 0;

    do
    {
        clear();
        printf("|------------------------------------|\n");
        printf("|             Welcome!               |\n");
        printf("|                                    |\n");
        printf("|Select one of the following options:|\n");
        printf("|    1 - ajouter utilisateure        |\n");
        printf("|    2 - afficher utilisateure       |\n");
        printf("|    3 - afficher Livre              |\n");
        printf("|    4 - ajouter Livre               |\n");
        printf("|    5 - Exit                        |\n");
        printf("|------------------------------------|\n");

        scanf(" %c", &option);

        switch(option)
        {
            case '1':
                Add_User(ls);
                break;

            case '2':
                Display_Users(ls);
                break;
            case '3':
               Display_Books(lv);
                break;
            case '4':
               Add_Book(lv);
                break;

            case '5':
                exit = 1;
                break;

            default:
                printf("Invalid option!\n\n");
                break;
        }
    }while(exit == 0);
}

void Menu_Student(User* this,BLIST* lv){
    char option;
    int exit = 0;

    do
    {
        clear();
        printf("|------------------------------------|\n");
        printf("|             Welcome!               |\n");
        printf("|                                    |\n");
        printf("|Select one of the following options:|\n");
        printf("|    1 - afficher utilisateure       |\n");
        printf("|    2 - afficher Livre              |\n");
        printf("|    3 - Exit                        |\n");
        printf("|------------------------------------|\n");

        scanf(" %c", &option);

        switch(option)
        {
            case '1':
                user_info(this);
                fgetc(stdin);
                fgetc(stdin);
                break;
            case '2':
                Display_Books(lv);
                break;

            case '3':
                exit = 1;
                break;

            default:
                printf("Invalid option!\n\n");
                break;
        }
    }while(exit == 0);
}

void initUser(ULIST* ls,char path[200]){
    load_User(ls,path);
    if(*ls == NULL){
        User* admin_dump = (User*) malloc(sizeof(User));
        admin_dump = Read_User();
        admin_dump->suivant = *ls;
        *ls = admin_dump;
    }

}

void initBooks(BLIST* lv){
    Livre* lv1 = makeBook("1234","blood and tears","Joe Doe");
    add_no_read(lv,lv1);

        Livre* lv2 = makeBook("443225","The thief","Joe Doe");
    add_no_read(lv,lv2);

        Livre* lv3 = makeBook("52565476","from earth to the moon","Jule vernne");
    add_no_read(lv,lv3);

    Livre* lv4 = makeBook("4454352","Let's just die","satan");
    add_no_read(lv,lv4);
    load_Books(lv,"books.lv");
}

void connect(ULIST* ls,BLIST* lv){
    char option;
    int exit = 0;
    char logging[10];
    char pass[20];
    User* log = (User*) malloc(sizeof(User));
    
    start:
    do
    {
        clear();
        printf("|------------------------------------|\n");
        printf("|           Welcome logging!         |\n");
        printf("|                                    |\n");
        printf("|Select one of the following options:|\n");
        printf("|    1 - CIN                         |\n");
        printf("|    2 - CNE                         |\n");
        printf("|    3 - APOGEE                      |\n");
        printf("|    4 - Exit                        |\n");
        printf("|------------------------------------|\n");

        scanf(" %c", &option);
        clear();
        switch(option)
        {
            case '1':
                printf("CIN : \n"); scanf("%s", logging);
                printf("password : \n");
                scanf("%s", pass);
                log = autho(ls,CIN,logging,pass);
                if(log == NULL) {
                    fprintf(stderr,"loggin or password wrong !!\n");
                    goto start;
                }
                if(log->type == ADMINISTRATEUR){
                    Menu_Admin(ls,lv);
                } else {
                    Menu_Student(log,lv);
                }
                break;

            case '2':
                printf("CNE : \n"); scanf("%s", logging);
                printf("password : \n"); scanf("%s", pass);
                log = autho(ls,CNE,logging,pass);
                if(log == NULL) {
                    fprintf(stderr,"loggin or password wrong !!\n");
                    goto start;
                }
                if(log->type == ADMINISTRATEUR){
                    Menu_Admin(ls,lv);
                } else {
                    Menu_Student(log,lv);
                }
                break;

            case '3':
                printf("APOGEE : \n"); scanf("%s", logging);
                printf("password : \n"); scanf("%s", pass);
                log = autho(ls,APOGEE,logging,pass);
                if(log == NULL) {
                    fprintf(stderr,"loggin or password wrong !!\n");
                    goto start;
                }
                if(log->type == ADMINISTRATEUR){
                    Menu_Admin(ls,lv);
                } else {
                    Menu_Student(log,lv);
                }
                break;
            case '4':
                exit = 1;
                break;
            default:
                printf("Invalid option!\n\n");
                break;
        }
    }while(exit == 0);
}





//*
int main(){
    ULIST* USER_LIST_1 = (ULIST*)malloc(sizeof(ULIST));
    *USER_LIST_1 = NULL;

    BLIST* BOOKS_1 = (BLIST*)malloc(sizeof(BLIST));
    *BOOKS_1 = NULL;
    initBooks(BOOKS_1);

    initUser(USER_LIST_1,"user.bin");
    connect(USER_LIST_1,BOOKS_1);


    return 0;
}//*/
