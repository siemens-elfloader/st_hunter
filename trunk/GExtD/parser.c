#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"


int IsRuleSymb(char *str){
    if(*str=='\r'||*str=='\n')return 1;
    if(*str=='/'&&*(str+1)=='*')return 2;
    if((*str=='\\'&&*(str+1)=='\\')||(*str==';'))return 3;
    return 0;
}

int PassMLComment(char *data){
    char *mass=data;
    int len=-1;
    if(*mass!='/')return len;
    mass++;
    if(*mass!='*')return len;
    mass++;
    len=0;
    while((*mass)&&(*mass!='*'&&*(mass+1)!='/')){
        len++;
        mass++;
    }
    return len;
}

int PassComment(char *data){
    char *mass=data;
    int len=-1;
    if(*mass!='\\'){
        if(*mass!=';')
        return len;
        mass++;
    }else{
    mass++;
    if(*mass!='\\')return len;
    mass++;
    }
    len=0;
    while(*mass&&!IsRuleSymb(mass)){
        mass++;
        len++;
    }
    return len;
}

int PassSpacesTab(char *data){
    char *temp=data;
    int len=0;
    while(*temp&&(*temp==' '||*temp=='\t')){
        len++;
        temp++;
    }
    return len;
}


SSIM *DoParse(char *mem){               //Функцию переписать
    char *temp=mem;
    int slen=0,addstr=0,scale=0;
    SSIM *root=NULL,*temps=NULL;

    temps=(SSIM *)malloc(sizeof(SSIM));
    memset(temps,0,sizeof(SSIM));
    root=temps;

    while(*temp){
        scale=1;
        addstr=0;
        if(*temp==' '){
            slen=PassSpacesTab(temp);
            temp+=slen;
            continue;
        }

        if(IsRuleSymb(temp)==1){*temp=0; temp++; continue;}

        if(IsRuleSymb(temp)==2&&!addstr){
            if((slen=PassMLComment(temp))>0){
                addstr=1;
                temps->comtype=MULTILINE_COMMENT;
                temps->str.len=slen;
                temps->str.str=temp+2;
            }
            *temp=0;
            *(temp+1)=0;
            if((*(temp+slen+2))=='*'){*(temp+slen+2)=0;}else addstr=-1;
            if((*(temp+slen+3))=='/'){*(temp+slen+3)=0;}else addstr=-1;
            scale=slen+4;
            slen=0;
        }

        if(IsRuleSymb(temp)==3&&!addstr){
            if((slen=PassComment(temp))>0){
                addstr=1;
                temps->comtype=ONELINE_COMMENT;
                temps->str.len=slen;
                temps->str.str=temp+2;
            }
            *temp=0;
            if((*(temp+1))=='\\'){
                *(temp+1)=0;
                scale=slen+2;
            }else scale=slen+1;
            slen=0;
        }

        if(*temp&&!addstr){
            addstr=1;
                temps->comtype=NONE_COMMENT;
                temps->str.str=temp;
                slen=0;
        while(!IsRuleSymb(temp)){
            slen++;
            temp++;
        }
                temps->str.len=slen;
                scale=0;

        }

        temp+=scale;
        if(addstr==1){
        temps->next=(SSIM *)malloc(sizeof(SSIM));
        memset(temps->next,0,sizeof(SSIM));
        temps->next->prev=temps;
        temps=temps->next;
        }
        }

        if(temps==root)root=NULL;
        else temps->prev->next=NULL;
        free(temps);

        return root;
}


void ParseClear(SSIM *root){
    SSIM *temp=root;
    while(root){
        temp=root;
        root=root->next;
//        if(temp->comment)free(temp->comment);
        free(temp);
    }
}
