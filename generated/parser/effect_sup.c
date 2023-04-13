#include "effect_sup.h"
#include <stdio.h>

extern size_t tabs;
extern effect_s current_effect;
extern char m_buff[2048];

char small_buffer[2048];

void putTab(size_t size){
    memset(small_buffer, ' ', 4*size);
    small_buffer[4*size] = '\0';
}

void build_search(char c){
    putTab(tabs);
    switch (current_effect.mark) {
        case EFF_PURE:
            sprintf(m_buff, "%sauto it_%s = effects.find({0, %s});\n", small_buffer, current_effect.name, current_effect.name);
            break;

        case EFF_SHARED:
            sprintf(m_buff, "%sauto it_%s_S = effects.find({1, %s});\n", small_buffer, current_effect.name, current_effect.name);
            break;
    
        case EFF_SHARED_ANY:
            sprintf(m_buff, "%sauto it_%s_S = effects.find({1, %s});\n%sauto it_%s_PS = effects_perm.find({1, %s});\n", small_buffer, current_effect.name, current_effect.name, small_buffer, current_effect.name, current_effect.name);
            break;

        case EFF_PROPERTY:
        case EFF_PERMANENT:
            sprintf(m_buff, "%sauto it_%s_P = effects_perm.find({0, %s});\n", small_buffer, current_effect.name, current_effect.name);
            break;
    
        case EFF_PERMANENT_ANY:
            sprintf(m_buff, "%sauto it_%s_P = effects_perm.find({0, %s});\n%sauto it_%s_PS = effects_perm.find({1, %s});\n", small_buffer, current_effect.name, current_effect.name, small_buffer, current_effect.name, current_effect.name);
            break;

        case EFF_PERMASHARED:
            sprintf(m_buff, "%sauto it_%s_PS = effects_perm.find({1, %s});\n", small_buffer, current_effect.name, current_effect.name);
            break;
            
        case EFF_ANY:
            sprintf(m_buff, "%sauto it_%s = effects.find({0, %s});\n%sauto it_%s_S = effects.find({1, %s});\n%sauto it_%s_P = effects_perm.find({0, %s});\n%sauto it_%s_PS = effects_perm.find({1, %s});\n", small_buffer, current_effect.name, current_effect.name, small_buffer, current_effect.name, current_effect.name, small_buffer, current_effect.name, current_effect.name, small_buffer, current_effect.name, current_effect.name);
            break;

        default:
            break;
    }

    char* tail = m_buff + strlen(m_buff);

    switch(current_effect.mark){
        case EFF_PURE:
            sprintf(tail, "%sif(%c(it_%s != effects.end())){", small_buffer, c, current_effect.name);
            break;

        case EFF_PROPERTY:
        case EFF_PERMANENT:
            sprintf(tail, "%sif(%c(it_%s_P != effects_perm.end())){", small_buffer, c, current_effect.name);
            break;

        case EFF_SHARED:
            sprintf(tail, "%sif(%c(it_%s_S != effects.end())){", small_buffer, c, current_effect.name);
            break;

        case EFF_PERMASHARED:
            sprintf(tail, "%sif(%c(it_%s_PS != effects_perm.end())){", small_buffer, c, current_effect.name);
            break;

        case EFF_ANY:
            sprintf(tail, "%sif(%c(it_%s != effects.end() || it_%s_S != effects.end() || it_%s_P != effects_perm.end() || it_%s_PS != effects_perm.end())){", small_buffer, c, current_effect.name, current_effect.name, current_effect.name, current_effect.name);
            break;

        case EFF_SHARED_ANY:
            sprintf(tail, "%sif(%c(it_%s_S != effects.end() || it_%s_PS != effects_perm.end())){", small_buffer, c, current_effect.name, current_effect.name);
            break;

        case EFF_PERMANENT_ANY:
            sprintf(tail, "%sif(%c(it_%s_P != effects_perm.end() || it_%s_PS != effects_perm.end())){", small_buffer, c, current_effect.name, current_effect.name);
            break;

        default:
            break;
    }
}

void impl_found(){
    // build_search(' ');
    // printf(m_buff);
    printf("find %s{", current_effect.name);
}
void impl_notfound(){ 
    // build_search('!');
    // printf(small_buffer);
    printf("neg_find %s{", current_effect.name);
}

void add_braces(){}

void build_expr(char* op){
    char* expr_end = strlen();

    switch(current_effect.mark){
        case EFF_PROPERTY:
        
    }
}

void impl_matan(){
    printf("matan{");
}

void impl_else(){
    printf("else{");
}

void checkTabs(size_t tab, size_t oldTab){
    size_t tabs2 = oldTab;
    while(tabs2 > tab){
        putchar('\n');
        putTab(--tabs2);
        putchar('}');
    }
    putTab(tab);
    if(oldTab > tabs) putchar('\n');
    printf(small_buffer);
}
