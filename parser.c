#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int Fsize=50;
int inputs=6;

int cases=10;


int i;
int j;


struct tableau {
    char *root;
    struct tableau *left;
    struct tableau *right;
    struct tableau *parent;
}*tab, *node, *node1, *kid, *pa;


char *tail(char *g)
{
    long len=strlen(g);
    char *t=malloc(sizeof(char)*(len-1));
    int i=0;
    int j=1;
    for(j;j<len;j++)
    {
        t[i]=g[j];
        i++;
    }
    return t;
}

char *partone(char *g)

{
    char *part_one=NULL;
    int brackettLeft=0;
    int brackettRight=0;
    for(int i=0; i<strlen(g);i++)
    {
        if(*(g+i)=='(')
        {
            brackettLeft++;
        }
        if(*(g+i)==')')
        {
            brackettRight++;
        }
        if (((brackettLeft - brackettRight) == 1)&& (*(g+i)=='>'||*(g+i)=='^'||*(g+i)=='v'))
        {
            part_one=malloc(sizeof(char)*i);
            strncpy(part_one,g+1,i-1);
            return part_one;
        }
    }
    return 0;
}

char *parttwo(char *g)

{
    
    char *part_two=NULL;
    int brackettLeft=0;
    int brackettRight=0;
    for(int i=0; i<strlen(g);i++)
    {
        if(*(g+i)=='(')
        {
            brackettLeft++;
        }
        if(*(g+i)==')')
        {
            brackettRight++;
        }
        if (((brackettLeft-brackettRight)==1)&& (*(g+i)=='>'||*(g+i)=='^'||*(g+i)=='v'))
        {
            part_two=malloc(sizeof(char)*(strlen(g)-i));
            strncpy(part_two,(g+i+1),(strlen(g)-i-2));
            return part_two;
        }
    }
    return 0;
    
}
char bin(char *g)

{
    int countLeft=0;
    int countRight=0;
    int i=0;
    for(i; i<strlen(g);i++)
    {
        if(*(g+i)=='(')
        {
            countLeft++;
        }
        if(*(g+i)==')')
        {
            countRight++;
        }
        if (((countLeft-countRight)==1)&& (*(g+i)=='>'||*(g+i)=='^'||*(g+i)=='v'))
        {
            return *(g+i);
        }
    }
    return 'd';
}


int parse(char *g)

{
    if (strlen(g) == 1 && ((*g='p')||(*g='q')||(*g='r')||(*g='s'))) {
        return 1;
    }
    if((*g=='-') && (parse(tail(g)) != 0))
    {
        return 2;
    }
    if((*g=='(') && (parse(partone(g)) != 0) && (parse(parttwo(g)) != 0) && (bin(g)=='^' || bin(g)=='>' || bin(g)=='v'))
    {
        return 3;
    }
    
    else
    {
        return 0;
    }
}

char *neg(char *g){
    if (parse(g)==2){
        return tail(g);
    }
    long len=strlen(g);
    char *n=malloc(sizeof(char)*(len+1));
    n[0]='-';
    int i=1;
    int j=0;
    for(j;j<len;j++){
        n[i]=g[j];
    }
    return n;
}


void addOne(struct tableau *t, char *g)
{
    if (t->left == NULL && t->right == NULL) {
        struct tableau *tp = malloc(sizeof(struct tableau));
        tp->root = g;
        tp->left = NULL;
        tp->right = NULL;
        tp->parent = t;
        
        t->left = tp;
    }
    else {
        if (t->left != NULL) {
            addOne(t->left, g);
        }
        if (t->right != NULL) {
            addOne(t->right, g);
        }
    }
}

void addTwo(struct tableau *tp, char *g, char *h)
{
    if (tp->left == NULL && tp->right == NULL) {
        
        struct tableau *l = malloc(sizeof(struct tableau));
        struct tableau *r = malloc(sizeof(struct tableau));
        
        l->root = g;
        l->left = NULL;
        l->right = NULL;
        l->parent = tp;
        tp->left = l;
        r->root = h;
        r->left = NULL;
        r->right = NULL;
        r->parent = tp;
        tp->right = r;
        
    }
    else {
        if (tp->left != NULL) {
            addTwo(tp->left, g, h);
        }
        if (tp->right != NULL) {
            addTwo(tp->right, g, h);
        }
    }
}

void complete(struct tableau *tp){
    if(tp!=NULL){
        if(tp->root != NULL){
            if(parse(tp->root)==3){ //binary and beta
                if( bin(tp->root)=='v' || bin(tp->root)=='>'){
                    addOne(tp, partone(tp->root));
                    addOne(tp, parttwo(tp->root));
                }
                if(bin(tp->root)=='^'){ //binary and alpha
                    addTwo(tp, partone(tp->root), parttwo(tp->root));
                }
            }
            if ( parse(tp->root) == 2 && parse(tail(tp->root)) != 2){
                if(bin(tail(tp->root))=='^'){ //alpha
                    addTwo( tp, neg(partone(tp->root)), neg(parttwo(tp->root)) );
                }
                if(bin(tail(tp->root))=='v'){
                    addOne(tp,neg(partone(tail(tp->root))));
                    addOne(tp,neg(parttwo(tail(tp->root))));
                }
                if(bin(tail(tp->root))=='>'){
                    addOne(tp,partone(tail(tp->root)));
                    addOne(tp,neg(parttwo(tail(tp->root))));
                }
            }
            if(parse(tp->root)==2 && parse(tail(tp->root)) == 2){
                addOne(tp, tail(tail(tp->root)));
            }
        }
        complete(tp->left);
        complete(tp->right);
    }
}

int label(struct tableau *tp, char *string) /*Is g label of current node or above?*/
{
    if (tp != NULL)
    {
        if ( strcmp(tp->root, string) == 0 ) {
            return label(tp->parent, string);
        }
        else {
            return 1;
        }
    }
    return 0;
}

int checkNeg(struct tableau *tp){
    if(parse(tp->root)==1){
        if (label(tp, neg(tp->root)) == 1) {
            return 1;
        }
        else {
            return checkNeg(tp->parent);
        }
    }
    return 0;
}

int closed(struct tableau *tp){
    if(parse(tp->root)==2 && parse(tail(tp->root))==2 && parse(tail(tail(tp->root)))==1){
        return 0;
    }
    if(checkNeg(tp)==1){
        return 0;
    }
    if(tp->left==NULL){
        return 1;
    }
    if(tp->right==NULL){
        return 0;
    }
    if(tp->right != NULL){
        return closed(tp->right);
    }
    if(tp->left !=NULL){
        return closed(tp->left);
    }
    return 0;
}



int main()

{ /*input a string and check if its a propositional formula */


  char *name = malloc(Fsize);
  FILE *fp, *fpout;
 
  /* reads from input.txt, writes to output.txt*/
  if ((  fp=fopen("input.txt","r"))==NULL){printf("Error opening file");exit(1);}
  if ((  fpout=fopen("output.txt","w"))==NULL){printf("Error opening file");exit(1);}

  int j;
  for(j=0;j<cases;j++)
    {
      fscanf(fp, "%s",name);/*read formula*/
      switch (parse(name))
    {case(0): fprintf(fpout, "%s is not a formula.  ", name);break;
    case(1): fprintf(fpout, "%s is a proposition.  ", name);break;
    case(2): fprintf(fpout, "%s is a negation.  ", name);break;
    case(3):fprintf(fpout, "%s is a binary.  ", name);break;
    default:fprintf(fpout, "What the f***!  ");
    }

      /*make new tableau with name at root, no children, no parent*/
  
      struct tableau t={name, NULL, NULL, NULL};

      /*expand the root, recursively complete the children*/
      if (parse(name)!=0)
    { complete(&t);
      if (closed(&t)) fprintf(fpout, "%s is not satisfiable.\n", name);
      else fprintf(fpout, "%s is satisfiable.\n", name);
    }
      else fprintf(fpout, "I told you, %s is not a formula.\n", name);
    }
 
  fclose(fp);
  fclose(fpout);
  free(name);

  return(0);
}