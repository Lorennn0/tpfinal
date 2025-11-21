//https://github.com/Lorennn0/tpfinal/new/main

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>

struct memory {
  char *response;
  size_t size;
};

int get_text_value(char *, char *, char *, int);



static size_t cb(char *data, size_t size, size_t nmemb, void *clientp)
{
  size_t realsize = nmemb;
  struct memory *mem = clientp;

  char *ptr = realloc(mem->response, mem->size + realsize + 1);
  if(!ptr)
    return 0;  /* out of memory */

  mem->response = ptr;
  memcpy(&(mem->response[mem->size]), data, realsize);
  mem->size += realsize;
  mem->response[mem->size] = 0;

  return realsize;
}

int main(void)
{
 char api_url[256];
 char *api_base = "https://api.telegram.org/bot";
 char token[50] = {0};
 char *cmd_getupdates = "/getUpdates";
 char *cmd_sendmessage1 = "/sendMessage?chat_id=";
 char *cmd_sendmessage2 = "&text=";
 char *cmd_offset = "?offset=";
 char nombre[40] = {0};
 char texto[40] = {0};
 long long update_id = 0;
 long long next_uid = 0;
 long long chat_id = 0;
 long long date = 0;

 FILE *fp=fopen("token.txt","r");

 if(fp!=NULL){

         fscanf(fp, "%s", token);
         fclose(fp);
 }
 else{
         printf("No se encontró el archivo");
 return 1;
 }



  CURLcode res;


  CURL *curl = curl_easy_init();


  while(1){

  snprintf(api_url, 256, "%s%s%s%s%lld",api_base,token,cmd_getupdates,cmd_offset,next_uid);
  struct memory chunk = {0};

  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, api_url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    res = curl_easy_perform(curl);
    if (res != 0)
      printf("Error Código: %d\n", res);


char *c_date = strstr(chunk.response, "\"date\":");

   if(c_date){
   c_date += strlen("\"date\":");
   date = atoll(c_date);


   FILE *pf=fopen("historial.txt","a");

    if(pf!=NULL){

    fprintf(pf, "\n%lld\n", date);
    fclose(pf);

    }
    else{
    printf("No se encontró el archivo");
    return 1;
    }
   }

//first name
get_text_value(nombre, chunk.response, "\"first_name\":\"", 40);

//texto
if(get_text_value(texto, chunk.response, "\"text\":\"", 40)){
if (strstr(texto, "hola") || strstr(texto, "Hola")){
snprintf(api_url, 256, "%s%s%s%lld%sHola,%%20%s",api_base,
                                                 token,
                                                 cmd_sendmessage1,
                                                 chat_id,
                                                 cmd_sendmessage2,
                                                 nombre);
    curl_easy_setopt(curl, CURLOPT_URL, api_url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    curl_easy_perform(curl);
     printf("%s\n", chunk.response);

     FILE *pf=fopen("historial.txt","a");
  if(pf!=NULL){

      fprintf(pf, "\n%s", nombre);
      fprintf(pf, "\n%s", texto);
      fclose(pf);

      }
      else{
      printf("No se encontró el archivo");
      return 1;
      }
}
if (strstr(texto, "chau") || strstr(texto, "Chau")){
snprintf(api_url, 256, "%s%s%s%lld%sAdios,%%20%s",api_base,
                                                 token,
                                                 cmd_sendmessage1,
                                                 chat_id,
                                                 cmd_sendmessage2,
                                                 nombre);
    curl_easy_setopt(curl, CURLOPT_URL, api_url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    curl_easy_perform(curl);
     printf("%s\n", chunk.response);
     FILE *pf=fopen("historial.txt","a");
       if(pf!=NULL){

           fprintf(pf, "\n%s", nombre);
           fprintf(pf, "\n%s", texto);
           fclose(pf);

           }
           else{
           printf("No se encontró el archivo");
           return 1;
           }
 }

 else{
 if (!(strstr(texto, "chau") || strstr(texto, "Chau") || strstr(texto, "Hola")||strstr(texto,"hola"))){
 snprintf(api_url, 256, "%s%s%s%lld%sNo%%20estoy%%20preparado%%20para%%20contestar%%20eso,%%20lo%%20siento,%%20%s",api_base,
                                                  token,
                                                  cmd_sendmessage1,
                                                  chat_id,
                                                  cmd_sendmessage2,
                                                  nombre);
     curl_easy_setopt(curl, CURLOPT_URL, api_url);
     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
     curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

     curl_easy_perform(curl);

 printf("%s\n", chunk.response);
 FILE *pf=fopen("historial.txt","a");
   if(pf!=NULL){

       fprintf(pf, "\n%s", nombre);
       fprintf(pf, "\n%s", texto);
       fclose(pf);

       }
       else{
       printf("No se encontró el archivo");
       return 1;
       }
 }
}
}


 char *c_id = strstr(chunk.response, "\"chat\":{\"id\":");

  if(c_id){
  c_id += strlen("\"chat\":{\"id\":");
  chat_id = atoll(c_id);
  }


  char *u_id = strstr(chunk.response, "\"update_id\":");

  if(u_id){
  u_id += strlen("\"update_id\":");
  update_id = atoll(u_id);
  next_uid = update_id+1;
  }
 else{

  printf("%s\n", chunk.response);
  }

    free(chunk.response);


  }


  sleep(2);
}
}


int get_text_value(char *dst, char *src, char *c, int n){
char *str = strstr(src,c);

if(str){
str += strlen(c);
char *end = strchr(str, '\"');
int _n = end - str;
if(n > _n)
strncpy(dst, str, _n);
return 1;

    }
}
