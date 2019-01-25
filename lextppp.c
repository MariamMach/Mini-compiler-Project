#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<stdio.h>

#define IDFSIZE 20 // taille maximal d'un identificateur
#define NUMBERSIZE 11 // talle maximal d'un nombre
#define COMMENTSIZE 30


typedef enum{
	//other tokens
		ID_TOKEN, INT_TOKEN, ERROR_TOKEN, REALINT_TOKEN, STRING_TOKEN,
	//keyword tokens
		PROGRAM_TOKEN, CONST_TOKEN, VAR_TOKEN, BEGIN_TOKEN, END_TOKEN, IF_TOKEN, THEN_TOKEN, WHILE_TOKEN, DO_TOKEN, READ_TOKEN, WRITE_TOKEN, 
	//symboles	
	PV_TOKEN, PT_TOKEN, PLUS_TOKEN, MOINS_TOKEN, DIV_TOKEN, MULT_TOKEN, AFF_TOKEN, INF_TOKEN, INFEG_TOKEN, SUP_TOKEN, SUPEG_TOKEN, DIFF_TOKEN, 
	PF_TOKEN, PO_TOKEN, FIN_TOKEN, VR_TOKEN, EG_TOKEN	
}CODES_LEX;



typedef struct token{
	CODES_LEX CODE;
    char NOM[20];
}token;

token nomToken[] = {{PROGRAM_TOKEN,"program"},{CONST_TOKEN,"const"},{VAR_TOKEN,"var"},{BEGIN_TOKEN, "begin"},{END_TOKEN,"end"}, {IF_TOKEN,"if"}, {THEN_TOKEN,"then"}, {WHILE_TOKEN, "while"}, {DO_TOKEN,"do"}, {READ_TOKEN, "read"}, {WRITE_TOKEN, "write"}};
token symboleToken[] = {{MULT_TOKEN,"*"}, {PT_TOKEN, "."}, {VR_TOKEN, ","}, {PV_TOKEN, ";"}, {PO_TOKEN, "("}, {PF_TOKEN,")"}, {MOINS_TOKEN, "-"}, {PLUS_TOKEN, "+"}, {DIV_TOKEN, "/"}};


typedef int bool;
#define true 1
#define false 0

token Sym_Cour;
int ligne = 1;

char* keyword_token[] = {"program", "const", "var", "begin", "end", "if", "then", "while", "do", "read", "write"};
char*  keyword_code[]={"PROGRAM_TOKEN", "CONST_TOKEN", "VAR_TOKEN", "BEGIN_TOKEN", "END_TOKEN", "IF_TOKEN", "THEN_TOKEN", "WHILE_TOKEN", "DO_TOKEN", "READ_TOKEN", "WRITE_TOKEN"};
int keyword_token_size = sizeof(keyword_token)/sizeof(keyword_token[0]); // la taille du tableau = 11

char symbole_token[] = {'*', '.', ',', ';', '(', ')', '-', '+','/'};
char* symbole_code[] = {"MULT_TOKEN", "PT_TOKEN", "VR_TOKEN", "PV_TOKEN","PO_TOKEN", "PF_TOKEN", "MOINS_TOKEN", "PLUS_TOKEN", "DIV_TOKEN"};

int symbole_token_size = sizeof(symbole_token)/sizeof(symbole_token[0]);


char* isSymboleCode(char c);
char* iskeyword(char* ch);
bool isAlphabetic(char c);
bool isNumeric(char c);
bool isSymbole(char c);
bool isBlanc(char car);
token lireFichier(FILE* fichier);


/*
const char nomfichier[] = "/home/meriem/Desktop/compilaV/test0.txt";
FILE* fichier = NULL;
*/

char* isSymboleCode(char c){
	int i;
	for(int i = 0; i < symbole_token_size; i++){
		if(c == symbole_token[i]){
			char* chaine1 = symbole_code[i];
			Sym_Cour.CODE = symboleToken[i].CODE;
			strcpy(Sym_Cour.NOM, symboleToken[i].NOM);
			return chaine1;
		}
	}
}


char* iskeyword(char* ch){
	int i;
	for(int i = 0; i < keyword_token_size; i++){
		if(strcmp(ch, keyword_token[i]) == 0){
			char* chaine1 = keyword_code[i];
			Sym_Cour.CODE = nomToken[i].CODE;
			strcpy(Sym_Cour.NOM, nomToken[i].NOM);
			return chaine1;
		}
	}
	Sym_Cour.CODE = ID_TOKEN;
	strcpy(Sym_Cour.NOM, ch);
	char* chaine2 = "ID_TOKEN";
	return chaine2;
}


bool isAlphabetic(char c){
     if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'){
        return true;
     }
     return false;
}


bool isNumeric(char c){
     if(c >= '0' && c <= '9'){
         return true;
     }
     return false;
}

bool isSymbole(char c){
          if(c == '+' || c == '(' || c == ')' || c == ';' || c == ',' || c == '&' || c == '*' || c == '.' || c == '/' || c == '-'){
               return true;
          }
    return false;
}

bool isBlanc(char car){
	if(car == ' ' || car == '\t' || car == '\r' ){
		return true;
	} else if( car == '\n' ){
			ligne++;
			return true;
	}
	return false;
}


token lireFichier(FILE* fichier){
			
			char car;
			car =  fgetc(fichier);
			int i = 0;
			char* number = (char *)malloc(NUMBERSIZE*sizeof(char));
			char* chaine = (char *)malloc(IDFSIZE*sizeof(char));
			
//traitement number
			if(isNumeric(car)){
				 do{
					 number[i] = car;
			 		 car = fgetc(fichier); // caractère suivant
					 i++;
		       		 }while (isNumeric(car));

				if(isAlphabetic(car)){
					do{
						number[i] = car;
			 			car = fgetc(fichier); // caractère suivant
						i++;
					}while(isAlphabetic(car));
					ungetc(car, fichier);
					number[i] = '\0';
					printf("	ERROR_TOKEN(%s) 😜😝😜\n", number);
					Sym_Cour.CODE = ERROR_TOKEN;
					strcpy(Sym_Cour.NOM, number);
					return Sym_Cour;

				}else if(car == '.'){
							do{
									 number[i] = car;
							 		 car = fgetc(fichier); // caractère suivant
									 i++;
				       		 }while (isNumeric(car));				       		 
							ungetc(car, fichier);
							number[i] = '\0';
							float val = atof(number);
							//printf("	REALINT_TOKEN(%f)\n", val);
							Sym_Cour.CODE = REALINT_TOKEN;
							strcpy(Sym_Cour.NOM, number);
							return Sym_Cour;
							

				}else{
					ungetc(car, fichier);
					 number[i] = '\0';
					int val = atoi(number);
					//printf("	INT_TOKEN(%d)\n", val);
					Sym_Cour.CODE = INT_TOKEN;
					strcpy(Sym_Cour.NOM, number);
					return Sym_Cour;
					
					free(number);

				}
//traitment id et keyword
			}else if(isAlphabetic(car)){
				 do{
					 chaine[i] = car;
			 		 car = fgetc(fichier); // caractère suivant
					 i++;
		       		 }while (isAlphabetic(car) == true);
				if(isNumeric(car)){
					do{
						chaine[i] = car;
			 		 	car = fgetc(fichier); // caractère suivant
					 	i++;
					}while(isNumeric(car) || isAlphabetic(car));
					ungetc(car, fichier);
					chaine[i] = '\0';
					iskeyword(chaine);
					//printf("	%s(%s)\n",iskeyword(chaine),  chaine);
					return Sym_Cour;	
				}else{
					ungetc(car, fichier);
					chaine[i] = '\0';
					iskeyword(chaine);
					//printf("	%s(%s)\n",iskeyword(chaine),  chaine);
					return Sym_Cour;
				}
				
			free(chaine);

//traitement symbole
			}else if(isSymbole(car)){
				isSymboleCode(car);
				//printf("	%s(%c)\n", isSymboleCode(car), car);				
				return Sym_Cour;

//traitement < <= <>
			}else if( car == '<'){
				car = fgetc(fichier); // caractère suivant
				if(car == '='){
					//printf("	INFEG_TOKEN(<=)\n");
					Sym_Cour.CODE = INFEG_TOKEN;
					strcpy(Sym_Cour.NOM, "<=");
					return Sym_Cour;
				}else if(car == '>'){
					//printf("	DIFF_TOKEN(<>)\n");
					Sym_Cour.CODE = DIFF_TOKEN;
					strcpy(Sym_Cour.NOM, "<>");
					return Sym_Cour;
				}else{
					ungetc(car, fichier);
					//printf("	INF_TOKEN(<)\n");
					Sym_Cour.CODE = INF_TOKEN;
					strcpy(Sym_Cour.NOM, "<");
					return Sym_Cour;
				}
//traitement > >=
			}else if( car == '>'){
				car = fgetc(fichier); // caractère suivant
				if(car == '='){
					//printf("	SUPEG_TOKEN(>=)\n");
					Sym_Cour.CODE = SUPEG_TOKEN;
					strcpy(Sym_Cour.NOM, "=>");
					return Sym_Cour;
				}else{
					ungetc(car, fichier);
					//printf("	SUP_TOKEN(>)\n");
					Sym_Cour.CODE = SUP_TOKEN;
					strcpy(Sym_Cour.NOM, ">");
					return Sym_Cour;
				}
				
			
//traitement :=
			}else if( car == ':'){
				car = fgetc(fichier); // caractère suivant
				if(car == '='){
					//printf("	AFF_TOKEN(:=)\n");
					Sym_Cour.CODE = AFF_TOKEN;
					strcpy(Sym_Cour.NOM, ":=");
					return Sym_Cour;

				}else{
					ungetc(car, fichier);
				}
//traitement =
			}else if(car == '='){
				//printf("	EG_TOKEN(=)\n");
				Sym_Cour.CODE = EG_TOKEN;
				strcpy(Sym_Cour.NOM, "=");
				return Sym_Cour;
				
// traitement commentaires {* *}
			}else if(car == '{'){
				 // caractère suivant
				if((car = fgetc(fichier)) == '*'){
					do{
						car = fgetc(fichier);
					}while(car != '*');
						if(fgetc(fichier) == '}'){
							//printf("	COMMENT_TOKEN()\n");
							return lireFichier(fichier);
						}					
				}else{
					ungetc(car, fichier);
					printf("	ERROR_TOEKN({) 😜😝😜\n");
					Sym_Cour.CODE = ERROR_TOKEN;
					strcpy(Sym_Cour.NOM, "{");
					return Sym_Cour;
				}
						
//caractere eof				
			}else if(car == EOF){
				//printf("	FIN_TOKEN(%c)\n", car);
				Sym_Cour.CODE = FIN_TOKEN;
				strcpy(Sym_Cour.NOM, "EOF");
				return Sym_Cour;

// caractere blanc			
			}else if(isBlanc(car)){
				do{
					car = fgetc(fichier);
				}while(isBlanc(car));
				ungetc(car, fichier);			
				return lireFichier(fichier);

// traitement string "....."
			}else if(car == '"'){
					char* chainestring = (char *)malloc(COMMENTSIZE*sizeof(char));
					int e = 0;
					do{
						car = fgetc(fichier);
						chainestring[e] = car;
						e++;
					}while(car != '"' && car != '\n');
						if(car == '"'){
							chainestring[e-1] = '\0';
							//printf("	STRING_TOKEN(%s)\n", chainestring);
							Sym_Cour.CODE = STRING_TOKEN;
							strcpy(Sym_Cour.NOM, chainestring);
							return Sym_Cour;
// string error	
						}else{
							chainestring[e-1] = '\0';
							printf("	ERROR_TOKEN(%s) 😜😝😜\n", chainestring);
							Sym_Cour.CODE = ERROR_TOKEN;
							strcpy(Sym_Cour.NOM, chainestring);
							return Sym_Cour;
						}
						
				free(chainestring);	
			} else if(car == 13){
					return lireFichier(fichier);

// traitement error
			}else{
					printf("	ERROR_TOKEN(%c) 😜😝😜\n", car);
					char str[2];
					str[0] = car;
					str[1] = '\0';
					Sym_Cour.CODE = ERROR_TOKEN;
					strcpy(Sym_Cour.NOM, str);
					return Sym_Cour;
			}

}


/*

int main(int argc, char *argv[]){
	fichier = fopen(nomfichier, "r");
	if(fichier != NULL){

			do{
				lireFichier(fichier);

			}while(Sym_Cour.CODE != FIN_TOKEN);

	}
	fclose(fichier);
	return 0;
}

*/

