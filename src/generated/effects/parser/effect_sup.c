#include "effect_sup.h"

extern size_t tabs;
extern effect_s current_effect;
extern char m_buff[2048];

char small_buffer[2048];
char arg_buffer[1024];

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
    build_search(' ');
    printf(m_buff);
    // printf("find %s{", current_effect.name);
    clean_buffers();
}
void impl_notfound(){ 
    build_search('!');
    printf(m_buff);
    // printf("neg_find %s{", current_effect.name);
    clean_buffers();
}

void add_braces(){
    m_buff[0] = '(';
    strcpy(small_buffer, m_buff);
    strcpy(m_buff+1, small_buffer);
    strcat(m_buff, ")");
}

void build_expr(char* op){
    char* end = m_buff + strlen(m_buff);
    if(op) *(end++) = *op;

    switch(current_effect.mark){
        case EFF_THIS:
            strcpy(end, "e.amount");
            break;

        case EFF_PURE:
            sprintf(end, "it_%s->second.amount", current_effect.name);
            break;

        case EFF_PERMANENT:
        case EFF_PROPERTY:
            sprintf(end, "it_%s_P->second", current_effect.name);
            break;

        case EFF_SHARED:
            sprintf(end, "it_%s_S->second.amount", current_effect.name);
            break;

        case EFF_PERMASHARED:
            sprintf(end, "it_%s_PS->second", current_effect.name);
            break;

        case EFF_NUMBER:
            strcpy(end, current_effect.name);
            break;

        case EFF_SHARED_ANY:
            sprintf(end, "((it_%s_S == effects.end() ? 0 : it_%s_S->second.amount) + (it_%s_PS == effects_perm.end() ? 0 : it_%s_PS->second))", current_effect.name, current_effect.name, current_effect.name, current_effect.name);
            break;

        case EFF_PERMANENT_ANY:
            sprintf(end, "((it_%s_P == effects_perm.end() ? 0 : it_%s_P->second) + (it_%s_PS == effects_perm.end() ? 0 : it_%s_PS->second))", current_effect.name, current_effect.name, current_effect.name, current_effect.name);
            break;

        case EFF_ANY:
            sprintf(end, "((it_%s == effects.end() ? 0 : it_%s->second.amount) + (it_%s_S == effects.end() ? 0 : it_%s_S->second.amount) + (it_%s_P == effects_perm.end() ? 0 : it_%s_P->second) + (it_%s_PS == effects_perm.end() ? 0 : it_%s_PS->second))", current_effect.name, current_effect.name, current_effect.name, current_effect.name, current_effect.name, current_effect.name, current_effect.name, current_effect.name);
            break;
    }
}

void impl_matan(){
    printf("if(%s){", m_buff);
    // printf("if(matan){");
    clean_buffers();
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
    small_buffer[0] = '\0';
}

// use construction of comments with log_construct() every times.
// do not forget clear it.

// tabs needed here
void write_comment(char* src){
    strcat(small_buffer, src);
    strcat(small_buffer, "\\n");
}

void put_sinle_effect_val(char* dest, effect_s* eff){
    switch(eff->mark){
        case EFF_PERMANENT:
        case EFF_PROPERTY:
            sprintf(dest, "it_%s_P->second", eff->name); return;

        case EFF_PURE:
            sprintf(dest, "it_%s->second.amount", eff->name); return;

        case EFF_SHARED:
            sprintf(dest, "it_%s_S->second.amount", eff->name); return;

        case EFF_PERMASHARED:
            sprintf(dest, "it_%s_PS->second", eff->name); return;

        case EFF_THIS:
            sprintf(dest, "e.amount");

        default: return;
    }
}

void write_comment_and_item(char* src){
    char* end = small_buffer + strlen(small_buffer);
    sprintf(end, "%s%%d", src);

    end = arg_buffer + strlen(arg_buffer);
    if(arg_buffer[0]){
        strcpy(end, ", ");
        end += 2;
    }

    put_sinle_effect_val(end, &current_effect);
}

void put_comment(){ 
    if(small_buffer[0]){
        if(arg_buffer[0]) printf("log_construct(buff, \"%s\", %s);\nstate::newline(buff);\n", small_buffer, arg_buffer);
        else printf("state::newline(\"%s\");\n", small_buffer);
    }
}

// todo
void impl_delete(){

}

void impl_set(effect_s eff, const char* op, size_t tabs){
    putTab(tabs);
    char* end = small_buffer + tabs * 4;
    put_sinle_effect_val(end, &eff);
    strcat(end, op);
    strcat(end, m_buff);
    strcat(end, ";\n");
    strcpy(m_buff, small_buffer);
    printf(m_buff);
    small_buffer[0] = '\0';
}

void impl_put_permanent(char* val, size_t tabs){
    putTab(tabs);
    char isShared = current_effect.mark == EFF_PERMASHARED ? '1' : '0';
    sprintf(m_buff, "%seffects_perm.insert({{%c, %s}, {%s}});", small_buffer, isShared, current_effect.name, val);
    small_buffer[0] = '\0';
}

void impl_put(char* val, char* time, size_t tabs){
    putTab(tabs);
    char isShared = current_effect.mark == EFF_SHARED ? '1' : '0';
    sprintf(m_buff, "%seffects.insert({{%c, %s}, {%s, %s}});", small_buffer, isShared, current_effect.name, val, time);
    small_buffer[0] = '\0';
}

void impl_raise_flag(char* flag){
    putTab(tabs);
    printf("this->flags.is_%s = true;\n", flag);
}

void impl_clear_flag(char* flag){
    putTab(tabs);
    printf("this->flags.is_%s = false;\n", flag);    
}

void impl_if_flag(char* flag){
    putTab(tabs);
    printf("if(this->flags.is_%s){\n", flag);
}

void impl_if_flag_neg(char* flag){
    putTab(tabs);
    printf("if(!this->flags.is_%s){\n", flag);
}

void clean_buffers(){
    small_buffer[0] = '\0';
    m_buff[0] = '\0';
    arg_buffer[0] = '\0';
}