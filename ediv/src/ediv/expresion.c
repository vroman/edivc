#include <string.h>

#include "main.h"
#include "compiler.h"
#include "parser.h"
#include "expresion.h"

void con00(int tipo_exp);
void con0();
void con1();
void con2();

void exp00(int tipo_exp);
void exp0();
void exp1();
void exp2();
void exp3();
void exp4();
void exp5();
void unario();
void exp6();

void factor(void);
void factor_struct(void);


int tipo_expresion=0; /* 0 - int, 1 - char o byte, 2 - string */
int tipo_factor=0;    /* idem */


/*
 *      Analisis de una condición (genera código para calcularla)
 */

void condicion(void) {
  byte * __ierror, * _ierror;
  int _linea;

  if (extended_conditions) { expresion(); return; } /* Como en DIV 1 */

  _exp=tabexp;
  save_error(0);
  _linea=linea; _ierror=ierror; con00(0);
  swap(linea,_linea); __ierror=ierror; ierror=_ierror;
  generar_expresion();
  linea=_linea; ierror=__ierror;
}

void con00(int tipo_exp) {
  int te=tipo_expresion;
  tipo_expresion=tipo_exp;
  con0();
  tipo_expresion=te;
}

void con0() { /* Operadores con asociatividad de derecha a izquierda <- */
  int p;

  con1();
  if ((p=pieza)==p_asig || (p>=p_add_asig && p<=p_shl_asig)) {
    error(0,69); /* asignación dentro de una condición */
  }
}

void con1() { /* Operadores con asociatividad de izquierda a derecha -> */
  int p;
  con2();
  while ((p=pieza)>=p_or && p<=p_andofs) {
    if (p>=p_xorptr) p-=p_xorptr-p_xor; /* Convierte xorptr,andofs en xor,and */
    lexico(); con2();
    (*_exp).tipo=eoper; (*_exp++).token=p;
    tipo_factor=0;
  }
}

void con2() {
  int p,tf;
  exp3();
  if (simple_conditions) {
    p=pieza;
    while ((p>=p_igu && p<=p_mai)||p==p_asig)  {
      tf=tipo_factor;
      if (p==p_asig) p=p_igu;
      lexico(); exp3();
      if (tf==2 || tipo_factor==2) {
        p+=p_strigu-p_igu;
        (*_exp).tipo=eoper; (*_exp++).token=p;
      } else {
        (*_exp).tipo=eoper; (*_exp++).token=p;
      } tipo_factor=0; p=pieza;
    }
  } else {
    while ((p=pieza)>=p_igu && p<=p_mai) {
      tf=tipo_factor;
      lexico(); exp3();
      if (tf==2 || tipo_factor==2) {
        p+=p_strigu-p_igu;
        (*_exp).tipo=eoper; (*_exp++).token=p;
      } else {
        (*_exp).tipo=eoper; (*_exp++).token=p;
      } tipo_factor=0;
    }
  }
}


/*
 *      Analisis de una expresión (genera código para calcularla)
 */
void expresion(void) {
  byte * __ierror, * _ierror;
  int _linea;

  _exp=tabexp;
  save_error(0);
  _linea=linea; _ierror=ierror; exp00(0);
  swap(linea,_linea); __ierror=ierror; ierror=_ierror;
  generar_expresion();
  linea=_linea; ierror=__ierror;
}


/* expresiones de parámetros */
void expresion_cpa(void) {
  byte * __ierror, * _ierror;
  int _linea;

  _exp=tabexp;
  save_error(0);
  _linea=linea; _ierror=ierror; exp00(0);
  swap(linea,_linea); __ierror=ierror; ierror=_ierror;
  if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_pointer) {
    _exp--; generar_expresion(); g1(lcpa);
  } else if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_pointerchar) {
    _exp--; generar_expresion();
    if (hacer_strfix) { g2(lcar,0); g1(lstrfix); g1(lasp); }
    g1(lcpachr);
  } else if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_pointerbyte) {
    _exp--; generar_expresion();
    g1(lcpachr);
  } else if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_pointerword) {
    _exp--; generar_expresion();
    g1(lcpawor);
  } else if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_string) {
    _exp--; generar_expresion(); g1(lcpastr);
  } else error(4,72); /* parámetro no válido */
  linea=_linea; ierror=__ierror;
}


void generar_expresion(void) {
  struct exp_ele * e=tabexp;
  struct objeto * ob;

  do {
    switch (e->tipo) {
      case econs:
        g2(lcar,e->valor); break;
      case estring:
        g2(lcar,e->valor); break;
      case erango:
        g2(lrng,e->valor); break;
      case ewhoami:
        g1(lcid); break;
      case ecall:
        ob=e->objeto; g2(lcal,ob->proc.offset);
        if (ob->usado) ob->proc.offset=imem-1; break;
      case efext:
        ob=e->objeto; g2(lext,ob->fext.codigo); break;
      case echeck:
        g1(lchk); break;
      case enull:
        g1(lnul); break;
      case eoper: switch(e->token) {
        case p_asig: g1(lasi); break;
        case p_or: g1(lori); break;
        case p_xor: g1(lxor); break;
        case p_and: g1(land); break;
        case p_igu: g1(ligu); break;
        case p_dis: g1(ldis); break;
        case p_may: g1(lmay); break;
        case p_men: g1(lmen); break;
        case p_mei: g1(lmei); break;
        case p_mai: g1(lmai); break;
        case p_add: g1(ladd); break;
        case p_sub: g1(lsub); break;
        case p_mul: g1(lmul); break;
        case p_div: g1(ldiv); break;
        case p_mod: g1(lmod); break;
        case p_neg: g1(lneg); break;
        case p_pointer: g1(lptr); break;
        case p_not: g1(lnot); break;
        case p_punto: g1(laid); break;
        case p_shr: g1(lshr); break;
        case p_shl: g1(lshl); break;
        case p_inc: g1(lipt); break;
        case p_suma: g1(lpti); break;
        case p_dec: g1(ldpt); break;
        case p_resta: g1(lptd); break;
        case p_add_asig: g1(lada); break;
        case p_sub_asig: g1(lsua); break;
        case p_mul_asig: g1(lmua); break;
        case p_div_asig: g1(ldia); break;
        case p_mod_asig: g1(lmoa); break;
        case p_and_asig: g1(lana); break;
        case p_or_asig: g1(lora); break;
        case p_xor_asig: g1(lxoa); break;
        case p_shr_asig: g1(lsra); break;
        case p_shl_asig: g1(lsla); break;

        case p_pointerword: g1(lptrwor); break;
        case p_sumaword: g1(lptiwor); break;
        case p_restaword: g1(lptdwor); break;
        case p_incword: g1(liptwor); break;
        case p_decword: g1(ldptwor); break;
        case p_asigword: g1(lasiwor); break;
        case p_add_asigword: g1(ladawor); break;
        case p_sub_asigword: g1(lsuawor); break;
        case p_mul_asigword: g1(lmuawor); break;
        case p_div_asigword: g1(ldiawor); break;
        case p_mod_asigword: g1(lmoawor); break;
        case p_and_asigword: g1(lanawor); break;
        case p_or_asigword: g1(lorawor); break;
        case p_xor_asigword: g1(lxoawor); break;
        case p_shr_asigword: g1(lsrawor); break;
        case p_shl_asigword: g1(lslawor); break;

        case p_pointerchar: g1(lptrchr); break;
        case p_sumachar: g1(lptichr); break;
        case p_restachar: g1(lptdchr); break;
        case p_incchar: g1(liptchr); break;
        case p_decchar: g1(ldptchr); break;
        case p_asigchar: g1(lasichr); break;
        case p_add_asigchar: g1(ladachr); break;
        case p_sub_asigchar: g1(lsuachr); break;
        case p_mul_asigchar: g1(lmuachr); break;
        case p_div_asigchar: g1(ldiachr); break;
        case p_mod_asigchar: g1(lmoachr); break;
        case p_and_asigchar: g1(lanachr); break;
        case p_or_asigchar: g1(lorachr); break;
        case p_xor_asigchar: g1(lxoachr); break;
        case p_shr_asigchar: g1(lsrachr); break;
        case p_shl_asigchar: g1(lslachr); break;

        case p_strcpy: g1(lstrcpy); break;
        case p_strfix: g1(lstrfix); break;
        case p_strcat: g1(lstrcat); break;
        case p_stradd: g1(lstradd); break;
        case p_strdec: g1(lstrdec); break;
        case p_strsub: g1(lstrsub); break;
        case p_strlen: g1(lstrlen); break;
        case p_strigu: g1(lstrigu); break;
        case p_strdis: g1(lstrdis); break;
        case p_strmay: g1(lstrmay); break;
        case p_strmen: g1(lstrmen); break;
        case p_strmei: g1(lstrmei); break;
        case p_strmai: g1(lstrmai); break;

        case p_pointerbyte: g1(lptrchr); break;
        case p_string: break; /* Es como un p_pointer nulo, que no se hace */

        default: error(4,11); /* expresión incorrecta */
      } break;
      default: error(4,11);
    }
  } while (++e!=_exp);
}


/*
 *      Analisis de una expresión constante
 *      No se procesan valores locales ni llamadas a otros procesos
 */
int constante (void) {

	byte * __ierror, * _ierror;
	struct exp_ele * e=_exp=tabexp;
	int _linea, i=0;
	
	save_error(0);

	_linea=linea; _ierror=ierror; exp00(0);
	swap(linea,_linea); __ierror=ierror; ierror=_ierror;

	do switch (e->tipo) {
    case econs:
      pila[++i]=e->valor; break;
    case estring:
      pila[++i]=e->valor; break;

    case eoper: switch(e->token) {
      case p_or: pila[i-1]|=pila[i]; i--; break;
      case p_xor: pila[i-1]^=pila[i]; i--; break;
      case p_and: pila[i-1]&=pila[i]; i--; break;
      case p_igu: pila[i-1]=pila[i-1]==pila[i]; i--; break;
      case p_dis: pila[i-1]=pila[i-1]!=pila[i]; i--; break;
      case p_may: pila[i-1]=pila[i-1]>pila[i]; i--; break;
      case p_men: pila[i-1]=pila[i-1]<pila[i]; i--; break;
      case p_mei: pila[i-1]=pila[i-1]<=pila[i]; i--; break;
      case p_mai: pila[i-1]=pila[i-1]>=pila[i]; i--; break;
      case p_add: pila[i-1]+=pila[i]; i--; break;
      case p_sub: pila[i-1]-=pila[i]; i--; break;
      case p_mul: pila[i-1]*=pila[i]; i--; break;
      case p_div: if (pila[i]==0) error(4,10); pila[i-1]/=pila[i]; i--; break;
      case p_mod: if (pila[i]==0) error(4,10); pila[i-1]%=pila[i]; i--; break;
      case p_neg: pila[i]=-pila[i]; break;
      case p_not: pila[i]^=-1; break;
      case p_shr: pila[i-1]>>=pila[i]; i--; break;
      case p_shl: pila[i-1]<<=pila[i]; i--; break;
      default: error(4,54);	/* se esperaba una expresión constante */
    } break;
    default: error(4,54);
  }while (++e!=_exp);

  linea=_linea; ierror=__ierror; return(pila[i]);
}

/*
 *      Análisis de una expresión a *tabexp
 */

void exp00(int tipo_exp) {
  int te=tipo_expresion;
  tipo_expresion=tipo_exp;
  exp0();
  tipo_expresion=te;
}

void exp0() { /* Operadores con asociatividad de derecha a izquierda <- */
  int p;

  exp1();
  if ((p=pieza)==p_asig || (p>=p_add_asig && p<=p_shl_asig)) {
    if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_pointer) {
      _exp--; lexico();
      exp00(0);
      (*_exp).tipo=eoper; (*_exp++).token=p;
    } else if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_pointerchar) {
      if (p==p_asig) p=p_asigchar; else p+=p_add_asigchar-p_add_asig;
      _exp--; lexico();
      exp00(1); /* cadena[1]="A"; */
      if (hacer_strfix) { (*_exp).tipo=eoper; (*_exp++).token=p_strfix; }
      (*_exp).tipo=eoper; (*_exp++).token=p;
    } else if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_pointerbyte) {
      if (p==p_asig) p=p_asigchar; else p+=p_add_asigchar-p_add_asig;
      _exp--; lexico();
      exp00(1); /* mis_bytes[1]="A"; */
      (*_exp).tipo=eoper; (*_exp++).token=p;
    } else if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_pointerword) {
      if (p==p_asig) p=p_asigword; else p+=p_add_asigword-p_add_asig;
      _exp--; lexico();
      exp00(0);
      (*_exp).tipo=eoper; (*_exp++).token=p;
    } else if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_string) {
      if (p==p_asig) { /* cadena="..." */ /* Hacer el strcpy */
        _exp--; lexico();
        exp00(2);
        (*_exp).tipo=eoper; (*_exp++).token=p_strcpy;
        (*_exp).tipo=eoper; (*_exp++).token=p_string; /* Sigue habiendo una cadena en la pila */
      } else if (p==p_add_asig) { /* cadena+="...";  Hacer el strcat */
        _exp--; lexico();
        exp00(2);
        (*_exp).tipo=eoper; (*_exp++).token=p_strcat;
        (*_exp).tipo=eoper; (*_exp++).token=p_string; /* Sigue habiendo una cadena en la pila */
      } else if (p==p_sub_asig) { /* cadena-=n; Quita caracteres */
        _exp--; lexico();
        exp00(-1);
        (*_exp).tipo=eoper; (*_exp++).token=p_strsub;
        (*_exp).tipo=eoper; (*_exp++).token=p_string; /* Sigue habiendo una cadena en la pila */
      } else error(0,12); /* asignación a cadena incorrecta */
    } else error(0,13); /* asignación incorrecta */
  } tipo_factor=0;
}

void exp1() { /* Operadores con asociatividad de izquierda a derecha -> */
  int p;
  exp2();
  while ((p=pieza)>=p_or && p<=p_andofs) {
    if (p>=p_xorptr) p-=p_xorptr-p_xor; /* Convierte xorptr,andofs en xor,and */
    lexico(); exp2();
    (*_exp).tipo=eoper; (*_exp++).token=p;
    tipo_factor=0;
  }
}

void exp2() {
  int p,tf;
  exp3();
  while ((p=pieza)>=p_igu && p<=p_mai) {
    tf=tipo_factor;
    lexico(); exp3();
    if (tf==2 || tipo_factor==2) {
      /* OJO, p==q no se hará el strcmp() si son dos punteros a cadenas */
      p+=p_strigu-p_igu;
      (*_exp).tipo=eoper; (*_exp++).token=p;
      tipo_factor=0;
    } else {
      (*_exp).tipo=eoper; (*_exp++).token=p;
    } tipo_factor=0;
  }
}

void exp3() {
  int p;
  exp4();
  while ((p=pieza)>=p_shr && p<=p_shl) {
    lexico(); exp4();
    (*_exp).tipo=eoper; (*_exp++).token=p;
    tipo_factor=0;
  }
}

void exp4() {
  int p,tf;
  exp5();
  while ((p=pieza)>=p_suma && p<=p_sub && (p<=p_resta || p>=p_add) ) {
    tf=tipo_factor;
    if (p<=p_resta) p+=p_add-p_suma; /* Convierte suma,resta en add,sub */
    lexico(); exp5();
    if (p==p_add && (tf==2 || tipo_factor==2 || tipo_expresion==2)) {
      (*_exp).tipo=eoper; (*_exp++).token=p_stradd;
      (*_exp).tipo=eoper; (*_exp++).token=p_string; /* Hay una cadena en la pila */
      tipo_factor=2;
    } else if (p==p_sub && tf==2) {
      if (tipo_factor==2) { (*_exp).tipo=eoper; (*_exp++).token=p_strlen; }
      (*_exp).tipo=eoper; (*_exp++).token=p_strdec;
      (*_exp).tipo=eoper; (*_exp++).token=p_string; /* Hay una cadena en la pila */
      tipo_factor=2;
    } else {
      (*_exp).tipo=eoper; (*_exp++).token=p;
      tipo_factor=0;
    }
  }
}

void exp5() {
  int p;
  unario();
  while ((p=pieza)>=p_mul && p<=p_multi) {
    if (p==p_multi) p=p_mul; /* Convierte multi en mul */
    lexico();  unario();
    (*_exp).tipo=eoper; (*_exp++).token=p;
    tipo_factor=0;
  }
}

void unario() {
  int p;

  while ((p=pieza)==p_add) { lexico();  }
  if ((p>=p_xorptr && p<=p_mul) || (p==p_inc || p==p_dec)) {

    if (parametros>0 && p==p_pointer) { factor(); return; }

    if (p>=p_sub && p<=p_mul) p-=p_sub-p_neg; /* Conv. sub,mul en neg,pointer */
    else if (p>=p_xorptr && p<=p_andofs) p+=p_pointer-p_xorptr; /* xorptr,andofs pointer,offset */

    lexico();  unario(); /* p = ( neg, pointer, offset, not ) */

    if (p==p_offset) {
      if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_pointer) _exp--;
      else if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_pointerchar) {
        error(4,14); /* &cadena[<exp>] ??? */
      } else if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_pointerbyte) {
        if ((*(_exp-2)).tipo==econs && (*(_exp-2)).valor==0) _exp-=2;
        else error(4,14); /* &bytes[<exp>] ??? */
      } else if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_pointerword) {
        if ((*(_exp-2)).tipo==econs && (*(_exp-2)).valor==0) _exp-=2;
        else error(4,14); /* &words[<exp>] ??? */
      } else if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_string) {
        /* &cadena ð cadena */
      } else error(4,15); /* no se puede calcular el offset */
    } else if (p==p_inc || p==p_dec) {
      if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_pointer) {
        _exp--;
        (*_exp).tipo=eoper; (*_exp++).token=p;
      } else if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_pointerchar) {
        _exp--;
        (*_exp).tipo=eoper; (*_exp++).token=p+p_incchar-p_inc;
      } else if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_pointerbyte) {
        _exp--;
        (*_exp).tipo=eoper; (*_exp++).token=p+p_incchar-p_inc;
      } else if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_pointerword) {
        _exp--;
        (*_exp).tipo=eoper; (*_exp++).token=p+p_incword-p_inc;
      } else if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_string) {
        _exp--;
        (*_exp).tipo=econs;
        if (p==p_inc) (*_exp++).valor=-1; else (*_exp++).valor=1;
        (*_exp).tipo=eoper; (*_exp++).token=p_strsub;
        (*_exp).tipo=eoper; (*_exp++).token=p_string;
      } else error(4,16); /* no se puede realizar el incremento */
    } else if (p==p_pointer) {
      if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_pointerchar) {
        /* *cadena[0] ð cadena[0] */
      } else if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_pointerbyte) {
        /* *cadena[0] ð cadena[0] */
      } else if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_pointerword) {
        /* *cadena[0] ð cadena[0] */
      } else if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_string) {
        /* *cadena == cadena */
      } else {
        (*_exp).tipo=eoper; (*_exp++).token=p;
      }
    } else {
      (*_exp).tipo=eoper; (*_exp++).token=p;
    }

  } else exp6();

}

void exp6() { /* Operador de acceso a variables o tablas locales ajenas */
  struct exp_ele * e;

  factor();
  while (pieza==p_punto) {
    if (comprueba_id) (*_exp++).tipo=echeck;
    e=_exp+1;
    acceso_remoto=1; lexico(); factor(); acceso_remoto=0;

    /* 
	 * OJO, el <p_punto> DEBE SER LO SEGUNDO que el <factor>
     * introduzca en la <expresión>, no quedan más cojones.
     * (bueno, si, hacer aquí una búsqueda del p_punto entre e-1 y _exp ...)
	 */

    if (e->tipo==eoper && e->token==p_punto)
      e->token=p_add; else error(4,17); /* local no se puede acceder */

  }
}

int struct_pointer; /* p_pointer / p_pointerchar / p_string / p_pointerbyte / p_pointerword */

void factor(void) {
  struct objeto * ob;
  struct objeto * obs;
  int p,offset,_imem;
  struct exp_ele * e;

  tipo_factor=0;

  switch (pieza) { /* analisis de un factor */

    case p_abrir:
      lexico(); exp00(tipo_expresion);
      if (pieza!=p_cerrar) error(3,18); lexico();  break;	/* esperando ')' */

    case p_corab:
      lexico();
      exp00(0);
      if (pieza!=p_corce) error(3,19);	/* esperando ']' */
      (*_exp).tipo=eoper; (*_exp++).token=p_pointer; lexico(); break;

    case p_whoami:
      (*_exp++).tipo=ewhoami; lexico();  break;

    case p_num:
      (*_exp).tipo=econs; (*_exp++).valor=pieza_num; lexico(); break;

    case p_lit:
      tipo_factor=2;
      if (tipo_expresion==1) {
        (*_exp).tipo=econs; (*_exp++).valor=(byte)mem[pieza_num];
      } else {
        (*_exp).tipo=estring; (*_exp++).valor=pieza_num;
        if (tipo_expresion==-1) { (*_exp).tipo=eoper; (*_exp++).token=p_strlen; }
      } lexico(); break;

    case p_type:
      lexico(); if (pieza!=p_id) error(0,21); /* esperando el nombre de un proceso */
      switch(o->tipo) {
        case tnone:
          o->linea=linea; o->ierror=ierror;
          (*_exp).tipo=econs; (*_exp++).valor=(int)o; o->usado=1; break;
        case tproc:
          (*_exp).tipo=econs; (*_exp++).valor=(int)o; break;
        case tsglo:
			if (!strcmp(o->name,"mouse")) { /* "type mouse" = "0"  */
            (*_exp).tipo=econs; (*_exp++).valor=0; break;
          }
        default: error(0,20); /* no es un tipo de proceso */
      } lexico(); break;

    case p_sizeof:

      lexico(); if (pieza!=p_abrir) error(3,22);	/* esperando '(' */
      lexico(); if (pieza!=p_id) error(3,23);		/* esperando un nombre */
      (*_exp).tipo=econs;
      switch(o->tipo) {
        case tsglo: case tsloc:
          (*_exp++).valor=o->sglo.len_item*o->sglo.totalitems; break;
        case tpsgl: case tpslo:
          (*_exp++).valor=(*(o->psgl.ostruct)).sglo.len_item*o->sglo.totalitems; break;
        case ttglo: case ttloc:
        case tbglo: case tbloc:
        case twglo: case twloc:
          (*_exp++).valor=o->tglo.totalen; break;
        case tcglo: case tcloc:
          (*_exp++).valor=(o->cglo.totalen+5)/4; break;
        case tvglo: case tvloc:
          (*_exp++).valor=1; break;
        default: error(0,24); /* no se puede calcular el tamaño */
      } lexico(); if (pieza!=p_cerrar) error(3,18); lexico();	/* esperando ')' */
      break;

    case p_struct: /* Se comprueba la declaración de un puntero a struct parámetro */
      if (parametros<=0) {
        if (error_25==25) error(1,25); else error(0,error_25);
      } else {
        lexico();
        if (pieza!=p_pointer) error(0,26); /* No se define el pointer así */
        lexico(); if (pieza!=p_id) error(1,27); obs=o;
        if (obs->tipo==tnone) error(0,28); /* No se define el pointer así */
        if (obs->tipo!=tsglo && obs->tipo!=tsloc) error(0,28);
        lexico(); ob=o;
        analiza_pointer_struct(tpslo,iloc++,obs);
        ob->linea=linea; ob->ierror=ierror;
        ob->param=1; parametros++;
        (*_exp).tipo=econs; (*_exp++).valor=ob->pslo.offset;
        (*_exp).tipo=eoper; (*_exp++).token=p_punto;
        (*_exp).tipo=eoper; (*_exp++).token=p_pointer;
      } break;

    case p_string: /* Se comprueba la declaración de un string parámetro */
      if (parametros<=0) {
        if (error_25==25) error(1,25); else error(0,error_25);
      } else {
        lexico();
        if (pieza==p_pointer) { /* Recibe un puntero a string */
          lexico();
          ob=analiza_pointer(tpclo,iloc++);
          ob->linea=linea; ob->ierror=ierror;
          ob->param=1; parametros++;
          (*_exp).tipo=econs; (*_exp++).valor=ob->pclo.offset;
          (*_exp).tipo=eoper; (*_exp++).token=p_punto;
          (*_exp).tipo=eoper; (*_exp++).token=p_pointer;
          break;
        }
        if (pieza!=p_id) error(1,29);	/* esperando el nombre de la cadena */
        ob=o; if (ob->tipo!=tnone) error(0,30);	/* el nombre no es nuevo */
        ob->linea=linea; ob->ierror=ierror;
        ob->param=1;
        ob->tipo=tcloc; lexico();
        if (pieza==p_corab) {
          lexico();
          if (pieza==p_corce) {
            lexico();
            ob->cloc.totalen=255;
          } else {
            e=_exp;
            if ((ob->cloc.totalen=constante())<0) error(4,31); /* cadena de long. negativa */
            if (ob->cloc.totalen>0xFFFFF) error(4,32); /* cadena demasiado larga */
            if (pieza!=p_corce) error(3,19);	/* esperando ']' */
            lexico();
            _exp=e;
          }
        } else ob->cloc.totalen=255;
        if (parametros>1) g2(lpar,parametros-1); parametros=1;
        g2(lpri,0); _imem=imem;
        imem+=1+(ob->cloc.totalen+5)/4;
        test_buffer(&mem,&imem_max,imem);
        mem[_imem]=0xDAD00000|ob->cloc.totalen;
        memset(&mem[imem+1],0,((ob->cloc.totalen+5)&-5));
        ob->cloc.offset=iloc+1;
        iloc+=1+(ob->cloc.totalen+5)/4;
        mem[_imem-1]=imem; // pri
        (*_exp).tipo=estring; (*_exp++).valor=ob->cloc.offset;
        (*_exp).tipo=eoper; (*_exp++).token=p_punto;
        (*_exp).tipo=eoper; (*_exp++).token=p_string;
      } break;

    case p_byte: /* Se comprueba la declaración de un byte parámetro */
      if (parametros<=0) {
        if (error_25==25) error(1,25); else error(0,error_25);
      } else {
        lexico();
        if (pieza==p_pointer) {
          lexico();
          ob=analiza_pointer(tpblo,iloc++);
          ob->linea=linea; ob->ierror=ierror;
          ob->param=1; parametros++;
          (*_exp).tipo=econs; (*_exp++).valor=ob->pblo.offset;
          (*_exp).tipo=eoper; (*_exp++).token=p_punto;
          (*_exp).tipo=eoper; (*_exp++).token=p_pointer;
          break;
        }
        if (pieza!=p_id) error(1,23); /* esperando un nombre */
        ob=o; if (ob->tipo!=tnone) error(0,30); /* el nombre no es nuevo */
        ob->linea=linea; ob->ierror=ierror;
        ob->param=1;
        ob->tipo=tbloc; lexico();
        if (pieza==p_corab) error(2,33); /* no se puede pasar una tabla como parametro */
        parametros++; ob->bloc.offset=iloc++;
        ob->bloc.len1=0;
        ob->bloc.len2=-1;
        ob->bloc.len3=-1;
        ob->bloc.totalen=1;
        (*_exp).tipo=econs; (*_exp++).valor=ob->bloc.offset;
        (*_exp).tipo=eoper; (*_exp++).token=p_punto;
        (*_exp).tipo=econs; (*_exp++).valor=0;
        (*_exp).tipo=eoper; (*_exp++).token=p_pointerbyte;
      } break;

    case p_word: /* Se comprueba la declaración de un word parámetro */
      if (parametros<=0) {
        if (error_25==25) error(1,25); else error(0,error_25);
      } else {
        lexico();
        if (pieza==p_pointer) {
          lexico();
          ob=analiza_pointer(tpwlo,iloc++);
          ob->linea=linea; ob->ierror=ierror;
          ob->param=1; parametros++;
          (*_exp).tipo=econs; (*_exp++).valor=ob->pwlo.offset;
          (*_exp).tipo=eoper; (*_exp++).token=p_punto;
          (*_exp).tipo=eoper; (*_exp++).token=p_pointer;
          break;
        }
        if (pieza!=p_id) error(1,23); /* esperando un nombre */
        ob=o; if (ob->tipo!=tnone) error(0,30); /* el nombre no es nuevo */
        ob->linea=linea; ob->ierror=ierror;
        ob->param=1;
        ob->tipo=twloc; lexico();
        if (pieza==p_corab) error(2,33); /* no se puede pasar una tabla como parametro */
        parametros++; ob->wloc.offset=iloc++;
        ob->wloc.len1=0;
        ob->wloc.len2=-1;
        ob->wloc.len3=-1;
        ob->wloc.totalen=1;
        (*_exp).tipo=econs; (*_exp++).valor=ob->wloc.offset;
        (*_exp).tipo=eoper; (*_exp++).token=p_punto;
        (*_exp).tipo=econs; (*_exp++).valor=0;
        (*_exp).tipo=eoper; (*_exp++).token=p_pointerword;
      } break;

    case p_int: /* Se comprueba la declaración de un int parámetro */
      if (parametros<=0) {
        if (error_25==25) error(1,25); else error(0,error_25);
      } else {
        lexico();
        if (pieza==p_pointer) {

    case p_pointer: /* Declaración de un puntero a int parámetro */
          if (parametros<=0) {
            if (error_25==25) error(1,25); else error(0,error_25);
          } parametros++;
          lexico();
          ob=analiza_pointer(tpilo,iloc++);
          ob->linea=linea; ob->ierror=ierror;
          ob->param=1;
          (*_exp).tipo=econs; (*_exp++).valor=ob->pilo.offset;
          (*_exp).tipo=eoper; (*_exp++).token=p_punto;
          (*_exp).tipo=eoper; (*_exp++).token=p_pointer;
          break;
        } else if (pieza!=p_id) error(1,23); /* esperando un nombre */
      }

    case p_id: switch (o->tipo) {

      case tnone:
        ob=o; ob->linea=linea; ob->ierror=ierror; lexico();
        if (pieza!=p_abrir) {
          if (parametros>0) {
            if (pieza==p_corab) error(2,33); /* no se puede pasar una tabla como parametro */
            parametros++;
            ob->tipo=tvloc; ob->vloc.offset=iloc++;
            ob->param=1;
            (*_exp).tipo=econs; (*_exp++).valor=ob->vloc.offset;
            (*_exp).tipo=eoper; (*_exp++).token=p_punto;
            (*_exp).tipo=eoper; (*_exp++).token=p_pointer; break;
          } else {
          	error(2,34,ob->name); /* nombre desconocido */
          	lexico();
          }
        }
        ob->usado=1; ob->tipo=tproc;
        ob->proc.bloque=ob; ob->proc.offset=0; ob->proc.num_par=0;
        lexico();
        while (pieza!=p_cerrar) {
          ob->proc.num_par++;
          exp00(0);
          if (pieza!=p_cerrar) {
          	if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
          	else { lexico(); if (pieza==p_cerrar) error(3,36); } /* se esperaba otro parametro */
          }
        } (*_exp).tipo=ecall; (*_exp++).objeto=ob; lexico();  break;

      case tcons:
        if (o->cons.literal) {
          tipo_factor=2;
          if (tipo_expresion==1) {
            (*_exp).tipo=econs; (*_exp++).valor=(byte)mem[pieza_num];
          } else if (tipo_expresion==-1) {
            (*_exp).tipo=econs; (*_exp++).valor=pieza_num;
            (*_exp).tipo=eoper; (*_exp++).token=p_strlen;
          } else {
            (*_exp).tipo=econs; (*_exp++).valor=o->cons.valor;
          }
        } else {
          (*_exp).tipo=econs; (*_exp++).valor=o->cons.valor;
        } lexico(); break;

      case tvglo:
        (*_exp).tipo=econs; (*_exp++).valor=o->vglo.offset;
        (*_exp).tipo=eoper; (*_exp++).token=p_pointer; lexico();  break;
        break;

      case ttglo:
      case tpigl:
        ob=o; offset=ob->tglo.offset;
        lexico();
        (*_exp).tipo=econs; (*_exp++).valor=offset;
        if (pieza==p_corab) {
          if (ob->tipo==tpigl) {
            (*_exp).tipo=eoper; (*_exp++).token=p_pointer;
            if (comprueba_null) (*_exp++).tipo=enull;
          }
          lexico();
          exp00(0);
          if (ob->tglo.len1>-1) if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->tglo.len1; }
          if (ob->tglo.len2>-1) {
            if (pieza!=p_coma) error(3,35); /* se esperaba una coma (multidimension) */
            lexico();
            exp00(0);
            if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->tglo.len2; }
            if (ob->tglo.len3>-1) {
              if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
              lexico();
              exp00(0);
              if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->tglo.len3; }
              (*_exp).tipo=econs; (*_exp++).valor=ob->tglo.len2+1;
              (*_exp).tipo=eoper; (*_exp++).token=p_mul;
              (*_exp).tipo=eoper; (*_exp++).token=p_add;
            }
            (*_exp).tipo=econs; (*_exp++).valor=ob->tglo.len1+1;
            (*_exp).tipo=eoper; (*_exp++).token=p_mul;
            (*_exp).tipo=eoper; (*_exp++).token=p_add;
          }
          if (pieza!=p_corce) error(3,19); /* esperando ']' */
          lexico();
          (*_exp).tipo=eoper; (*_exp++).token=p_add;
        } (*_exp).tipo=eoper; (*_exp++).token=p_pointer; break;

      case tbglo:
      case tpbgl:
      case tpcgl:
        ob=o; offset=ob->bglo.offset;
        lexico();
        (*_exp).tipo=econs; (*_exp++).valor=offset;
        if (ob->tipo==tpbgl || ob->tipo==tpcgl) {
          (*_exp).tipo=eoper; (*_exp++).token=p_pointer;
          if (pieza!=p_corab) break;
          if (comprueba_null) (*_exp++).tipo=enull;
        }
        if (pieza==p_corab) {
          lexico();
          exp00(0);
          if (ob->bglo.len1>-1) if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->bglo.len1; }
          if (ob->bglo.len2>-1) {
            if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
            lexico();
            exp00(0);
            if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->bglo.len2; }
            if (ob->bglo.len3>-1) {
              if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
              lexico();
              exp00(0);
              if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->bglo.len3; }
              (*_exp).tipo=econs; (*_exp++).valor=ob->bglo.len2+1;
              (*_exp).tipo=eoper; (*_exp++).token=p_mul;
              (*_exp).tipo=eoper; (*_exp++).token=p_add;
            }
            (*_exp).tipo=econs; (*_exp++).valor=ob->bglo.len1+1;
            (*_exp).tipo=eoper; (*_exp++).token=p_mul;
            (*_exp).tipo=eoper; (*_exp++).token=p_add;
          }
          if (pieza!=p_corce) error(3,19); /* esperando ']' */
          lexico();
        } else { /* mi_byte ð mi_byte[0] */
          (*_exp).tipo=econs; (*_exp++).valor=0;
        } (*_exp).tipo=eoper; (*_exp++).token=p_pointerbyte ; break;

      case twglo:
      case tpwgl:
        ob=o; offset=ob->wglo.offset;
        lexico();
        (*_exp).tipo=econs; (*_exp++).valor=offset;
        if (ob->tipo==tpwgl) {
          (*_exp).tipo=eoper; (*_exp++).token=p_pointer;
          if (pieza!=p_corab) break;
          if (comprueba_null) (*_exp++).tipo=enull;
        }
        if (pieza==p_corab) {
          lexico();
          exp00(0);
          if (ob->wglo.len1>-1) if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->wglo.len1; }
          if (ob->wglo.len2>-1) {
            if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
            lexico();
            exp00(0);
            if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->wglo.len2; }
            if (ob->wglo.len3>-1) {
              if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
              lexico();
              exp00(0);
              if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->wglo.len3; }
              (*_exp).tipo=econs; (*_exp++).valor=ob->wglo.len2+1;
              (*_exp).tipo=eoper; (*_exp++).token=p_mul;
              (*_exp).tipo=eoper; (*_exp++).token=p_add;
            }
            (*_exp).tipo=econs; (*_exp++).valor=ob->wglo.len1+1;
            (*_exp).tipo=eoper; (*_exp++).token=p_mul;
            (*_exp).tipo=eoper; (*_exp++).token=p_add;
          }
          if (pieza!=p_corce) error(3,19); /* esperando ']' */
          lexico();
        } else { /* mi_byte ð mi_byte[0] */
          (*_exp).tipo=econs; (*_exp++).valor=0;
        } (*_exp).tipo=eoper; (*_exp++).token=p_pointerword; break;

      case tcglo:
        ob=o; offset=ob->cglo.offset;
        lexico();
        (*_exp).tipo=estring; (*_exp++).valor=offset;
        if (pieza==p_corab) {
          lexico();
          exp00(0);
          if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->cglo.totalen; }
          if (pieza!=p_corce) error(3,19); /* esperando ']' */
          lexico();
          (*_exp).tipo=eoper; (*_exp++).token=p_pointerchar;
        } else {
          tipo_factor=2;
          if (tipo_expresion==1) {
            (*_exp).tipo=econs; (*_exp++).valor=0;
            (*_exp).tipo=eoper; (*_exp++).token=p_pointerchar;
          } else {
            (*_exp).tipo=eoper; (*_exp++).token=p_string;
            if (tipo_expresion==-1) { (*_exp).tipo=eoper; (*_exp++).token=p_strlen; }
          }
        } break;

      case tsglo:
      case tpsgl:
        ob=o; lexico(); (*_exp).tipo=econs; (*_exp++).valor=ob->sglo.offset;
        if (ob->tipo==tpsgl) {
          (*_exp).tipo=eoper; (*_exp++).token=p_pointer;
          if (pieza!=p_corab && pieza!=p_punto) break;
          if (comprueba_null) (*_exp++).tipo=enull;
        }
        if (pieza==p_corab) {
          lexico();
          exp00(0);
          if (ob->sglo.items1>-1) if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->sglo.items1; }
          if (ob->sglo.items2>-1) {
            if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
            lexico();
            exp00(0);
            if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->sglo.items2; }
            if (ob->sglo.items3>-1) {
              if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
              lexico();
              exp00(0);
              if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->sglo.items3; }
              (*_exp).tipo=econs; (*_exp++).valor=ob->sglo.items2+1;
              (*_exp).tipo=eoper; (*_exp++).token=p_mul;
              (*_exp).tipo=eoper; (*_exp++).token=p_add;
            }
            (*_exp).tipo=econs; (*_exp++).valor=ob->sglo.items1+1;
            (*_exp).tipo=eoper; (*_exp++).token=p_mul;
            (*_exp).tipo=eoper; (*_exp++).token=p_add;
          }
          if (pieza!=p_corce) error(3,19); lexico(); /* esperando ']' */
          if (ob->tipo==tpsgl) {
            (*_exp).tipo=econs; (*_exp++).valor=(*(ob->psgl.ostruct)).sglo.len_item;
          } else {
            (*_exp).tipo=econs; (*_exp++).valor=ob->sglo.len_item;
          }
          (*_exp).tipo=eoper; (*_exp++).token=p_mul;
          (*_exp).tipo=eoper; (*_exp++).token=p_add;
        }
        if (pieza==p_punto) {
          struct_pointer=p_pointer;
          if (ob->tipo==tpsgl) member=ob->psgl.ostruct; else member=ob;
          lexico(); factor_struct();
          (*_exp).tipo=eoper; (*_exp++).token=struct_pointer;
        } else {
          (*_exp).tipo=eoper; (*_exp++).token=p_pointer;
        } break;

      case tvloc:
        if (acceso_remoto && o->bloque) error(0,37); /* no se puede acceder a PRIVATE externos */
        (*_exp).tipo=econs; (*_exp++).valor=o->vloc.offset;
        (*_exp).tipo=eoper; (*_exp++).token=p_punto;
        (*_exp).tipo=eoper; (*_exp++).token=p_pointer; lexico(); break;

      case ttloc:
      case tpilo:
        if (acceso_remoto && o->bloque) error(0,37); /* no se puede acceder a PRIVATE externos */
        ob=o; offset=ob->tloc.offset;
        lexico();
        (*_exp).tipo=econs; (*_exp++).valor=offset;
        (*_exp).tipo=eoper; (*_exp++).token=p_punto;
        if (pieza==p_corab) {
          if (ob->tipo==tpilo) {
            (*_exp).tipo=eoper; (*_exp++).token=p_pointer;
            if (comprueba_null) (*_exp++).tipo=enull;
          }
          acceso_remoto=0;
          lexico();
          exp00(0);
          if (ob->tloc.len1>-1) if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->tloc.len1; }
          if (ob->tloc.len2>-1) {
            if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
            lexico();
            exp00(0);
            if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->tloc.len2; }
            if (ob->tloc.len3>-1) {
              if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
              lexico();
              exp00(0);
              if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->tloc.len3; }
              (*_exp).tipo=econs; (*_exp++).valor=ob->tloc.len2+1;
              (*_exp).tipo=eoper; (*_exp++).token=p_mul;
              (*_exp).tipo=eoper; (*_exp++).token=p_add;
            }
            (*_exp).tipo=econs; (*_exp++).valor=ob->tloc.len1+1;
            (*_exp).tipo=eoper; (*_exp++).token=p_mul;
            (*_exp).tipo=eoper; (*_exp++).token=p_add;
          }
          if (pieza!=p_corce) error(3,19); /* esperando ']' */
          lexico();
          (*_exp).tipo=eoper; (*_exp++).token=p_add;
        }
        (*_exp).tipo=eoper; (*_exp++).token=p_pointer; break;

      case tbloc:
      case tpblo:
      case tpclo:
        if (acceso_remoto && o->bloque) error(0,37); /* no se puede acceder a PRIVATE externos */
        ob=o; offset=ob->bloc.offset;
        lexico();
        (*_exp).tipo=econs; (*_exp++).valor=offset;
        (*_exp).tipo=eoper; (*_exp++).token=p_punto;
        if (ob->tipo==tpblo || ob->tipo==tpclo) {
          (*_exp).tipo=eoper; (*_exp++).token=p_pointer;
          if (pieza!=p_corab) break;
          if (comprueba_null) (*_exp++).tipo=enull;
        }
        if (pieza==p_corab) {
          acceso_remoto=0;
          lexico();
          exp00(0);
          if (ob->bloc.len1>-1) if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->bloc.len1; }
          if (ob->bloc.len2>-1) {
            if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
            lexico();
            exp00(0);
            if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->bloc.len2; }
            if (ob->bloc.len3>-1) {
              if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
              lexico();
              exp00(0);
              if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->bloc.len3; }
              (*_exp).tipo=econs; (*_exp++).valor=ob->bloc.len2+1;
              (*_exp).tipo=eoper; (*_exp++).token=p_mul;
              (*_exp).tipo=eoper; (*_exp++).token=p_add;
            }
            (*_exp).tipo=econs; (*_exp++).valor=ob->bloc.len1+1;
            (*_exp).tipo=eoper; (*_exp++).token=p_mul;
            (*_exp).tipo=eoper; (*_exp++).token=p_add;
          }
          if (pieza!=p_corce) error(3,19); /* esperando ']' */
          lexico();
        } else { /* mi_byte ð mi_byte[0] */
          (*_exp).tipo=econs; (*_exp++).valor=0;
        } (*_exp).tipo=eoper; (*_exp++).token=p_pointerbyte ; break;

      case twloc:
      case tpwlo:
        if (acceso_remoto && o->bloque) error(0,37); /* no se puede acceder a PRIVATE externos */
        ob=o; offset=ob->wloc.offset;
        lexico();
        (*_exp).tipo=econs; (*_exp++).valor=offset;
        (*_exp).tipo=eoper; (*_exp++).token=p_punto;
        if (ob->tipo==tpwlo) {
          (*_exp).tipo=eoper; (*_exp++).token=p_pointer;
          if (pieza!=p_corab) break;
          if (comprueba_null) (*_exp++).tipo=enull;
        }
        if (pieza==p_corab) {
          acceso_remoto=0;
          lexico();
          exp00(0);
          if (ob->wloc.len1>-1) if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->wloc.len1; }
          if (ob->wloc.len2>-1) {
            if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
            lexico();
            exp00(0);
            if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->wloc.len2; }
            if (ob->wloc.len3>-1) {
              if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
              lexico();
              exp00(0);
              if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->wloc.len3; }
              (*_exp).tipo=econs; (*_exp++).valor=ob->wloc.len2+1;
              (*_exp).tipo=eoper; (*_exp++).token=p_mul;
              (*_exp).tipo=eoper; (*_exp++).token=p_add;
            }
            (*_exp).tipo=econs; (*_exp++).valor=ob->wloc.len1+1;
            (*_exp).tipo=eoper; (*_exp++).token=p_mul;
            (*_exp).tipo=eoper; (*_exp++).token=p_add;
          }
          if (pieza!=p_corce) error(3,19); /* esperando ']' */
          lexico();
        } else { /* mi_byte ð mi_byte[0] */
          (*_exp).tipo=econs; (*_exp++).valor=0;
        } (*_exp).tipo=eoper; (*_exp++).token=p_pointerword; break;

      case tcloc:
        if (acceso_remoto && o->bloque) error(0,37);  /* no se puede acceder a PRIVATE externos */
        ob=o; offset=ob->cloc.offset;
        lexico();
        (*_exp).tipo=estring; (*_exp++).valor=offset;
        (*_exp).tipo=eoper; (*_exp++).token=p_punto;
        if (pieza==p_corab) {
          acceso_remoto=0;
          lexico();
          exp00(0);
          if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->cloc.totalen; }
          if (pieza!=p_corce) error(3,19); /* esperando ']' */
          lexico();
          (*_exp).tipo=eoper; (*_exp++).token=p_pointerchar;
        } else {
          tipo_factor=2;
          if (tipo_expresion==1) {
            (*_exp).tipo=econs; (*_exp++).valor=0;
            (*_exp).tipo=eoper; (*_exp++).token=p_pointerchar;
          } else {
            (*_exp).tipo=eoper; (*_exp++).token=p_string;
            if (tipo_expresion==-1) { (*_exp).tipo=eoper; (*_exp++).token=p_strlen; }
          }
        } break;

      case tsloc:
      case tpslo:
        if (acceso_remoto && o->bloque) error(0,37); /* no se puede acceder a PRIVATE externos */
        ob=o; lexico(); (*_exp).tipo=econs; (*_exp++).valor=ob->sloc.offset;
        (*_exp).tipo=eoper; (*_exp++).token=p_punto;
        if (ob->tipo==tpslo) {
          (*_exp).tipo=eoper; (*_exp++).token=p_pointer;
          if (pieza!=p_corab && pieza!=p_punto) break;
          if (comprueba_null) (*_exp++).tipo=enull;
        }
        if (pieza==p_corab) {
          acceso_remoto=0;
          lexico();
          exp00(0);
          if (ob->sloc.items1>-1) if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->sloc.items1; }
          if (ob->sloc.items2>-1) {
            if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
            lexico();
            exp00(0);
            if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->sloc.items2; }
            if (ob->sloc.items3>-1) {
              if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
              lexico();
              exp00(0);
              if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->sloc.items3; }
              (*_exp).tipo=econs; (*_exp++).valor=ob->sloc.items2+1;
              (*_exp).tipo=eoper; (*_exp++).token=p_mul;
              (*_exp).tipo=eoper; (*_exp++).token=p_add;
            }
            (*_exp).tipo=econs; (*_exp++).valor=ob->sloc.items1+1;
            (*_exp).tipo=eoper; (*_exp++).token=p_mul;
            (*_exp).tipo=eoper; (*_exp++).token=p_add;
          }
          if (pieza!=p_corce) error(3,19); lexico(); /* esperando ']' */
          if (ob->tipo==tpslo) {
            (*_exp).tipo=econs; (*_exp++).valor=(*(ob->pslo.ostruct)).sloc.len_item;
          } else {
            (*_exp).tipo=econs; (*_exp++).valor=ob->sloc.len_item;
          }
          (*_exp).tipo=eoper; (*_exp++).token=p_mul;
          (*_exp).tipo=eoper; (*_exp++).token=p_add;
        }
        if (pieza==p_punto) {
          struct_pointer=p_pointer;
          if (ob->tipo==tpslo) member=ob->psgl.ostruct; else member=ob;
          lexico(); factor_struct();
          (*_exp).tipo=eoper; (*_exp++).token=struct_pointer;
        } else {
          (*_exp).tipo=eoper; (*_exp++).token=p_pointer;
        } break;

      case tproc:
        ob=o; lexico(); if (pieza!=p_abrir) error(3,22); lexico(); /* esperando '(' */
        p=0; while (pieza!=p_cerrar) { /* pðnum_par */
          p++;
          exp00(0);
          if (pieza!=p_cerrar) {
          	if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
          	else { lexico(); if (pieza==p_cerrar) error(3,36); } /* se esperaba otro parametro */
          }
        }
        if (p!=ob->proc.num_par) error(1,38); /* numero de parametros incorrecto */
        (*_exp).tipo=ecall; (*_exp++).objeto=ob; lexico(); break;

      // POR HACER: implementar soporte para qsort (ver comentario aquí arriba)

      case tfext:
        ob=o; lexico(); if (pieza!=p_abrir) error(3,22); /* esperando '(' */
        lexico();
        p=0; while (pieza!=p_cerrar) { /* pðnum_par */
          p++;
          exp00(0);
          if (pieza!=p_cerrar) {
          	if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
          	else { lexico(); if (pieza==p_cerrar) error(3,36); } /* se esperaba otro parametro */
          }
        }

		while(1) {
			if(ob==NULL) break;
			if(ob->tipo==tfext && ob->fext.num_par==p) break;
			ob=ob->anterior;
		}
		if(ob==NULL) error(1,38); /* numero de parametros incorrecto */
        (*_exp).tipo=efext; (*_exp++).objeto=ob; lexico(); break;

      default:
        if (error_25==25) error(1,25); else error(0,error_25);
        break;

    } break;

    default:
      if (error_25==25) error(1,25); else error(0,error_25);
      break;
  }

  if (pieza==p_inc || pieza==p_dec) {
    if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_pointer) {
      _exp--;
      (*_exp).tipo=eoper;
      if (pieza==p_inc) (*_exp++).token=p_suma;
        else (*_exp++).token=p_resta;
    } else if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_pointerchar) {
      _exp--;
      (*_exp).tipo=eoper;
      if (pieza==p_inc) (*_exp++).token=p_sumachar;
        else (*_exp++).token=p_restachar;
    } else if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_pointerbyte) {
      _exp--;
      (*_exp).tipo=eoper;
      if (pieza==p_inc) (*_exp++).token=p_sumachar;
        else (*_exp++).token=p_restachar;
    } else if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_pointerword) {
      _exp--;
      (*_exp).tipo=eoper;
      if (pieza==p_inc) (*_exp++).token=p_sumaword;
        else (*_exp++).token=p_restaword;
    } else if ((*(_exp-1)).tipo==eoper && (*(_exp-1)).token==p_string) {
      _exp--; (*_exp).tipo=econs;
      if (pieza==p_inc) (*_exp++).valor=-1;
        else (*_exp++).valor=1;
      (*_exp).tipo=eoper;
      (*_exp++).token=p_strsub;
      (*_exp).tipo=eoper;
      (*_exp++).token=p_string;
    } else error(4,16); /* no se puede realizar el incremento */
    lexico();
  }
}


void factor_struct(void) {
  struct objeto * ob;

  if (pieza!=p_id) error(3,39); /* esperando un elemento de la estructura */
  switch (o->tipo) {

    case tvglo:
      (*_exp).tipo=econs; (*_exp++).valor=o->vglo.offset;
      (*_exp).tipo=eoper; (*_exp++).token=p_add;
      member=NULL; lexico();
      break;

    case ttglo:
    case tpigl:
      (*_exp).tipo=econs; (*_exp++).valor=o->tglo.offset; ob=o;
      (*_exp).tipo=eoper; (*_exp++).token=p_add;
      member=NULL; lexico();
      if (pieza==p_corab) {
        if (ob->tipo==tpigl) {
          (*_exp).tipo=eoper; (*_exp++).token=p_pointer;
          if (comprueba_null) (*_exp++).tipo=enull;
        }
        lexico();
        exp00(0);
        if (ob->tglo.len1>-1) if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->tglo.len1; }
        if (ob->tglo.len2>-1) {
          if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
          lexico();
          exp00(0);
          if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->tglo.len2; }
          if (ob->tglo.len3>-1) {
            if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
            lexico();
            exp00(0);
            if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->tglo.len3; }
            (*_exp).tipo=econs; (*_exp++).valor=ob->tglo.len2+1;
            (*_exp).tipo=eoper; (*_exp++).token=p_mul;
            (*_exp).tipo=eoper; (*_exp++).token=p_add;
          }
          (*_exp).tipo=econs; (*_exp++).valor=ob->tglo.len1+1;
          (*_exp).tipo=eoper; (*_exp++).token=p_mul;
          (*_exp).tipo=eoper; (*_exp++).token=p_add;
        }
        if (pieza!=p_corce) error(3,19); lexico(); /* esperando ']' */
        (*_exp).tipo=eoper; (*_exp++).token=p_add;
      } break;

    case tbglo:
    case tpbgl:
    case tpcgl:
      (*_exp).tipo=econs; (*_exp++).valor=o->bglo.offset; ob=o;
      (*_exp).tipo=eoper; (*_exp++).token=p_add;
      member=NULL; lexico();
      if (ob->tipo==tpbgl || ob->tipo==tpcgl) {
        if (pieza!=p_corab) break;
        (*_exp).tipo=eoper; (*_exp++).token=p_pointer;
        if (comprueba_null) (*_exp++).tipo=enull;
      }
      if (pieza==p_corab) {
        lexico();
        exp00(0);
        if (ob->bglo.len1>-1) if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->bglo.len1; }
        if (ob->bglo.len2>-1) {
          if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
          lexico();
          exp00(0);
          if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->bglo.len2; }
          if (ob->bglo.len3>-1) {
            if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
            lexico();
            exp00(0);
            if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->bglo.len3; }
            (*_exp).tipo=econs; (*_exp++).valor=ob->bglo.len2+1;
            (*_exp).tipo=eoper; (*_exp++).token=p_mul;
            (*_exp).tipo=eoper; (*_exp++).token=p_add;
          }
          (*_exp).tipo=econs; (*_exp++).valor=ob->bglo.len1+1;
          (*_exp).tipo=eoper; (*_exp++).token=p_mul;
          (*_exp).tipo=eoper; (*_exp++).token=p_add;
        }
        if (pieza!=p_corce) error(3,19); lexico(); /* esperando ']' */
      } else { /* mi_byte ð mi_byte[0] */
        (*_exp).tipo=econs; (*_exp++).valor=0;
      } struct_pointer=p_pointerbyte; break;

    case twglo:
    case tpwgl:
      (*_exp).tipo=econs; (*_exp++).valor=o->wglo.offset; ob=o;
      (*_exp).tipo=eoper; (*_exp++).token=p_add;
      member=NULL; lexico();
      if (ob->tipo==tpwgl) {
        if (pieza!=p_corab) break;
        (*_exp).tipo=eoper; (*_exp++).token=p_pointer;
        if (comprueba_null) (*_exp++).tipo=enull;
      }
      if (pieza==p_corab) {
        lexico();
        exp00(0);
        if (ob->wglo.len1>-1) if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->wglo.len1; }
        if (ob->wglo.len2>-1) {
          if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
          lexico();
          exp00(0);
          if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->wglo.len2; }
          if (ob->wglo.len3>-1) {
            if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
            lexico();
            exp00(0);
            if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->wglo.len3; }
            (*_exp).tipo=econs; (*_exp++).valor=ob->wglo.len2+1;
            (*_exp).tipo=eoper; (*_exp++).token=p_mul;
            (*_exp).tipo=eoper; (*_exp++).token=p_add;
          }
          (*_exp).tipo=econs; (*_exp++).valor=ob->wglo.len1+1;
          (*_exp).tipo=eoper; (*_exp++).token=p_mul;
          (*_exp).tipo=eoper; (*_exp++).token=p_add;
        }
        if (pieza!=p_corce) error(3,19); lexico(); /* esperando ']' */
      } else { /* mi_byte ð mi_byte[0] */
        (*_exp).tipo=econs; (*_exp++).valor=0;
      } struct_pointer=p_pointerword; break;

    case tcglo:
      (*_exp).tipo=estring; (*_exp++).valor=o->cglo.offset; ob=o;
      (*_exp).tipo=eoper; (*_exp++).token=p_add;
      member=NULL; lexico();
      if (pieza==p_corab) {
        lexico();
        exp00(0);
        if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->cglo.totalen; }
        if (pieza!=p_corce) error(3,19); /* esperando ']' */
        lexico();
        struct_pointer=p_pointerchar;
      } else {
        tipo_factor=2;
        if (tipo_expresion==1) {
          (*_exp).tipo=econs; (*_exp++).valor=0;
          struct_pointer=p_pointerchar;
        } else {
          if (tipo_expresion==-1) { (*_exp).tipo=eoper; (*_exp++).token=p_strlen; }
          struct_pointer=p_string;
        }
      } break;

    case tsglo:
    case tpsgl:
      (*_exp).tipo=econs; (*_exp++).valor=o->sglo.offset; ob=o;
      (*_exp).tipo=eoper; (*_exp++).token=p_add;
      member=NULL; lexico();
      if (ob->tipo==tpsgl) {
        if (pieza!=p_corab && pieza!=p_punto) break;
        (*_exp).tipo=eoper; (*_exp++).token=p_pointer;
        if (comprueba_null) (*_exp++).tipo=enull;
      }
      if (pieza==p_corab) {
        lexico();
        exp00(0);
        if (ob->sglo.items1>-1) if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->sglo.items1; }
        if (ob->sglo.items2>-1) {
          if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
          lexico();
          exp00(0);
          if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->sglo.items2; }
          if (ob->sglo.items3>-1) {
            if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
            lexico();
            exp00(0);
            if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->sglo.items3; }
            (*_exp).tipo=econs; (*_exp++).valor=ob->sglo.items2+1;
            (*_exp).tipo=eoper; (*_exp++).token=p_mul;
            (*_exp).tipo=eoper; (*_exp++).token=p_add;
          }
          (*_exp).tipo=econs; (*_exp++).valor=ob->sglo.items1+1;
          (*_exp).tipo=eoper; (*_exp++).token=p_mul;
          (*_exp).tipo=eoper; (*_exp++).token=p_add;
        }
        if (pieza!=p_corce) error(3,19); lexico(); /* esperando ']' */
        if (ob->tipo==tpsgl) {
          (*_exp).tipo=econs; (*_exp++).valor=(*(ob->psgl.ostruct)).sglo.len_item;
        } else {
          (*_exp).tipo=econs; (*_exp++).valor=ob->sglo.len_item;
        }
        (*_exp).tipo=eoper; (*_exp++).token=p_mul;
        (*_exp).tipo=eoper; (*_exp++).token=p_add;
      }
      if (pieza==p_punto) {
        if (ob->tipo==tpsgl) member=ob->psgl.ostruct; else member=ob;
        lexico(); factor_struct();
      } break;

    case tvloc:
      (*_exp).tipo=econs; (*_exp++).valor=o->vloc.offset;
      (*_exp).tipo=eoper; (*_exp++).token=p_add;
      member=NULL; lexico(); break;

    case ttloc:
    case tpilo:
      (*_exp).tipo=econs; (*_exp++).valor=o->tloc.offset; ob=o;
      (*_exp).tipo=eoper; (*_exp++).token=p_add;
      member=NULL; lexico();
      if (pieza==p_corab) {
        if (ob->tipo==tpilo) {
          (*_exp).tipo=eoper; (*_exp++).token=p_pointer;
          if (comprueba_null) (*_exp++).tipo=enull;
        }
        lexico();
        exp00(0);
        if (ob->tloc.len1>-1) if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->tloc.len1; }
        if (ob->tloc.len2>-1) {
          if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
          lexico();
          exp00(0);
          if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->tloc.len2; }
          if (ob->tloc.len3>-1) {
            if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
            lexico();
            exp00(0);
            if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->tloc.len3; }
            (*_exp).tipo=econs; (*_exp++).valor=ob->tloc.len2+1;
            (*_exp).tipo=eoper; (*_exp++).token=p_mul;
            (*_exp).tipo=eoper; (*_exp++).token=p_add;
          }
          (*_exp).tipo=econs; (*_exp++).valor=ob->tloc.len1+1;
          (*_exp).tipo=eoper; (*_exp++).token=p_mul;
          (*_exp).tipo=eoper; (*_exp++).token=p_add;
        }
        if (pieza!=p_corce) error(3,19); lexico(); /* esperando ']' */
        (*_exp).tipo=eoper; (*_exp++).token=p_add;
      } break;

    case tbloc:
    case tpblo:
    case tpclo:
      (*_exp).tipo=econs; (*_exp++).valor=o->bloc.offset; ob=o;
      (*_exp).tipo=eoper; (*_exp++).token=p_add;
      member=NULL; lexico();
      if (ob->tipo==tpblo || ob->tipo==tpcgl) {
        if (pieza!=p_corab) break;
        (*_exp).tipo=eoper; (*_exp++).token=p_pointer;
        if (comprueba_null) (*_exp++).tipo=enull;
      }
      if (pieza==p_corab) {
        lexico();
        exp00(0);
        if (ob->bloc.len1>-1) if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->bloc.len1; }
        if (ob->bloc.len2>-1) {
          if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
          lexico();
          exp00(0);
          if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->bloc.len2; }
          if (ob->bloc.len3>-1) {
            if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
            lexico();
            exp00(0);
            if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->bloc.len3; }
            (*_exp).tipo=econs; (*_exp++).valor=ob->bloc.len2+1;
            (*_exp).tipo=eoper; (*_exp++).token=p_mul;
            (*_exp).tipo=eoper; (*_exp++).token=p_add;
          }
          (*_exp).tipo=econs; (*_exp++).valor=ob->bloc.len1+1;
          (*_exp).tipo=eoper; (*_exp++).token=p_mul;
          (*_exp).tipo=eoper; (*_exp++).token=p_add;
        }
        if (pieza!=p_corce) error(3,19); lexico(); /* esperando ']' */
      } else { /* mi_byte ð mi_byte[0] */
        (*_exp).tipo=econs; (*_exp++).valor=0;
      } struct_pointer=p_pointerbyte; break;

    case twloc:
    case tpwlo:
      (*_exp).tipo=econs; (*_exp++).valor=o->wloc.offset; ob=o;
      (*_exp).tipo=eoper; (*_exp++).token=p_add;
      member=NULL; lexico();
      if (ob->tipo==tpwlo) {
        if (pieza!=p_corab) break;
        (*_exp).tipo=eoper; (*_exp++).token=p_pointer;
        if (comprueba_null) (*_exp++).tipo=enull;
      }
      if (pieza==p_corab) {
        lexico();
        exp00(0);
        if (ob->wloc.len1>-1) if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->wloc.len1; }
        if (ob->wloc.len2>-1) {
          if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
          lexico();
          exp00(0);
          if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->wloc.len2; }
          if (ob->wloc.len3>-1) {
            if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
            lexico();
            exp00(0);
            if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->wloc.len3; }
            (*_exp).tipo=econs; (*_exp++).valor=ob->wloc.len2+1;
            (*_exp).tipo=eoper; (*_exp++).token=p_mul;
            (*_exp).tipo=eoper; (*_exp++).token=p_add;
          }
          (*_exp).tipo=econs; (*_exp++).valor=ob->wloc.len1+1;
          (*_exp).tipo=eoper; (*_exp++).token=p_mul;
          (*_exp).tipo=eoper; (*_exp++).token=p_add;
        }
        if (pieza!=p_corce) error(3,19); lexico(); /* esperando ']' */
      } else { /* mi_byte ð mi_byte[0] */
        (*_exp).tipo=econs; (*_exp++).valor=0;
      } struct_pointer=p_pointerword; break;

    case tcloc:
      (*_exp).tipo=estring; (*_exp++).valor=o->cloc.offset; ob=o;
      (*_exp).tipo=eoper; (*_exp++).token=p_add;
      member=NULL; lexico();
      if (pieza==p_corab) {
        lexico();
        exp00(0);
        if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->cloc.totalen; }
        if (pieza!=p_corce) error(3,19); /* esperando ']' */
        lexico();
        struct_pointer=p_pointerchar;
      } else {
        tipo_factor=2;
        if (tipo_expresion==1) {
          (*_exp).tipo=econs; (*_exp++).valor=0;
          struct_pointer=p_pointerchar;
        } else {
          if (tipo_expresion==-1) { (*_exp).tipo=eoper; (*_exp++).token=p_strlen; }
          struct_pointer=p_string;
        }
      } break;

    case tsloc:
    case tpslo:
      (*_exp).tipo=econs; (*_exp++).valor=o->sloc.offset; ob=o;
      (*_exp).tipo=eoper; (*_exp++).token=p_add;
      member=NULL; lexico();
      if (ob->tipo==tpslo) {
        if (pieza!=p_corab && pieza!=p_punto) break;
        (*_exp).tipo=eoper; (*_exp++).token=p_pointer;
        if (comprueba_null) (*_exp++).tipo=enull;
      }
      if (pieza==p_corab) {
        lexico();
        exp00(0);
        if (ob->sloc.items1>-1) if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->sloc.items1; }
        if (ob->sloc.items2>-1) {
          if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
          lexico();
          exp00(0);
          if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->sloc.items2; }
          if (ob->sloc.items3>-1) {
            if (pieza!=p_coma) error(3,35); /* se esperaba una coma */
            lexico();
            exp00(0);
            if (comprueba_rango) { (*_exp).tipo=erango; (*_exp++).valor=ob->sloc.items3; }
            (*_exp).tipo=econs; (*_exp++).valor=ob->sloc.items2+1;
            (*_exp).tipo=eoper; (*_exp++).token=p_mul;
            (*_exp).tipo=eoper; (*_exp++).token=p_add;
          }
          (*_exp).tipo=econs; (*_exp++).valor=ob->sloc.items1+1;
          (*_exp).tipo=eoper; (*_exp++).token=p_mul;
          (*_exp).tipo=eoper; (*_exp++).token=p_add;
        }
        if (pieza!=p_corce) error(3,19); lexico(); /* esperando ']' */
        if (ob->tipo==tpslo) {
          (*_exp).tipo=econs; (*_exp++).valor=(*(ob->pslo.ostruct)).sloc.len_item;
        } else {
          (*_exp).tipo=econs; (*_exp++).valor=ob->sloc.len_item;
        }
        (*_exp).tipo=eoper; (*_exp++).token=p_mul;
        (*_exp).tipo=eoper; (*_exp++).token=p_add;
      }
      if (pieza==p_punto) {
        if (ob->tipo==tpslo) member=ob->pslo.ostruct; else member=ob;
        lexico(); factor_struct();
      } break;

    default: error(3,39); break; /* esperando un elemento de la estructura */
  }
}
