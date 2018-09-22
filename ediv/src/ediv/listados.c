/*
 * eDiv Compiler
 * Copyright (C) 2000-2002 Sion, Ltd.
 * http://www.sionhq.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "parser.h"
#include "compiler.h"

#include "language.h"

/*
 * Graba el fichero de la tabla de objetos
 */

void listado_objetos(void)
{
  FILE *sta;
  char nombretab[256];
  int n, o, l;
  char *msg_37 = get_translate(27, fichero_prg);

  strcpy(nombretab, nombreprog);
  strcat(nombretab, ".tab");

  if ((sta = fopen(nombretab, "w")) == NULL)
  {
    print_translate(36, nombretab);
    exit(1);
  }

  fprintf(sta, msg_37);
  free(msg_37);

  for (n = 1; n <= 8; n++)
    fprintf(sta, "[%u]: %u\n", n, mem[n]);
  fprintf(sta, "\n");

  for (n = 0; n < num_obj; n++)
  {
    switch (obj[n].tipo)
    {
    case tnone:
      fprintf(sta, "%5u\ttnone: %s", n, obj[n].name);
      if (obj[n].usado)
        fprintf(sta, " (*) usado\n");
      else
        fprintf(sta, "\n");
      break;
    case tcons:
      fprintf(sta, "%5u\ttcons: %s", n, obj[n].name);
      if (obj[n].usado)
        fprintf(sta, " (*) usado\n");
      else
        fprintf(sta, "\n");
      fprintf(sta, "\tvalor=%u\n", obj[n].cons.valor);
      break;
    case tvglo:
      fprintf(sta, "%5u\ttvglo: %s", n, obj[n].name);
      if (obj[n].usado)
        fprintf(sta, " (*) usado\n");
      else
        fprintf(sta, "\n");
      fprintf(sta, "\toffset=%u\n", o = obj[n].vglo.offset);
      if (obj[n].member)
        fprintf(sta, "\tmember of %s\n", (*obj[n].member).name);
      else
        fprintf(sta, "\tvalor=%u\n", mem[o]);
      break;
    case ttglo:
      fprintf(sta, "%5u\tttglo: %s", n, obj[n].name);
      if (obj[n].usado)
        fprintf(sta, " (*) usado\n");
      else
        fprintf(sta, "\n");
      fprintf(sta, "\toffset=%u\n", o = obj[n].tglo.offset);
      fprintf(sta, "\tlen1=%u\n", obj[n].tglo.len1);
      fprintf(sta, "\tlen2=%u\n", obj[n].tglo.len2);
      fprintf(sta, "\tlen3=%u\n", obj[n].tglo.len3);
      fprintf(sta, "\ttotalen=%u\n", obj[n].tglo.totalen);
      if (obj[n].member)
        fprintf(sta, "\tmember of %s\n", (*obj[n].member).name);
      break;
    case tpbgl:
    case tpwgl:
    case tpcgl:
    case tpigl:
      fprintf(sta, "%5u\ttp?gl: %s", n, obj[n].name);
      if (obj[n].usado)
        fprintf(sta, " (*) usado\n");
      else
        fprintf(sta, "\n");
      fprintf(sta, "\toffset=%u\n", o = obj[n].tglo.offset);
      fprintf(sta, "\tlen1=%u\n", obj[n].tglo.len1);
      fprintf(sta, "\tlen2=%u\n", obj[n].tglo.len2);
      fprintf(sta, "\tlen3=%u\n", obj[n].tglo.len3);
      fprintf(sta, "\ttotalen=%u\n", obj[n].tglo.totalen);
      if (obj[n].member)
        fprintf(sta, "\tmember of %s\n", (*obj[n].member).name);
      break;
    case tsglo:
      fprintf(sta, "%5u\ttsglo: %s", n, obj[n].name);
      if (obj[n].usado)
        fprintf(sta, " (*) usado\n");
      else
        fprintf(sta, "\n");
      fprintf(sta, "\toffset=%u\n", o = obj[n].sglo.offset);
      fprintf(sta, "\titems1=%u\n", obj[n].sglo.items1);
      fprintf(sta, "\titems2=%u\n", obj[n].sglo.items2);
      fprintf(sta, "\titems3=%u\n", obj[n].sglo.items3);
      fprintf(sta, "\ttotalitems=%u\n", l = obj[n].sglo.totalitems);
      fprintf(sta, "\tlen_item=%u\n", obj[n].sglo.len_item);
      if (obj[n].member)
        fprintf(sta, "\tmember of %s\n", (*obj[n].member).name);
      break;
    case tpsgl:
      fprintf(sta, "%5u\ttpsgl: %s", n, obj[n].name);
      if (obj[n].usado)
        fprintf(sta, " (*) usado\n");
      else
        fprintf(sta, "\n");
      fprintf(sta, "\toffset=%u\n", o = obj[n].sglo.offset);
      fprintf(sta, "\titems1=%u\n", obj[n].sglo.items1);
      fprintf(sta, "\titems2=%u\n", obj[n].sglo.items2);
      fprintf(sta, "\titems3=%u\n", obj[n].sglo.items3);
      fprintf(sta, "\ttotalitems=%u\n", l = obj[n].sglo.totalitems);
      fprintf(sta, "\tlen_item=%u\n", obj[n].sglo.len_item);
      if (obj[n].member)
        fprintf(sta, "\tmember of %s\n", (*obj[n].member).name);
      break;
    case tvloc:
      fprintf(sta, "%5u\ttvloc: %s", n, obj[n].name);
      if (obj[n].usado)
        fprintf(sta, " (*) usado\n");
      else
        fprintf(sta, "\n");
      fprintf(sta, "\toffset=%u\n", o = obj[n].vloc.offset);
      if (obj[n].member)
        fprintf(sta, "\tmember of %s\n", (*obj[n].member).name);
      else
        fprintf(sta, "\tvalor=%u\n", loc[o]);
      break;
    case ttloc:
      fprintf(sta, "%5u\tttloc: %s", n, obj[n].name);
      if (obj[n].usado)
        fprintf(sta, " (*) usado\n");
      else
        fprintf(sta, "\n");
      fprintf(sta, "\toffset=%u\n", o = obj[n].tloc.offset);
      fprintf(sta, "\tlen1=%u\n", obj[n].tloc.len1);
      fprintf(sta, "\tlen2=%u\n", obj[n].tloc.len2);
      fprintf(sta, "\tlen3=%u\n", obj[n].tloc.len3);
      fprintf(sta, "\ttotalen=%u\n", obj[n].tloc.totalen);
      if (obj[n].member)
        fprintf(sta, "\tmember of %s\n", (*obj[n].member).name);
      break;
    case tpblo:
    case tpwlo:
    case tpclo:
    case tpilo:
      fprintf(sta, "%5u\ttp?lo: %s", n, obj[n].name);
      if (obj[n].usado)
        fprintf(sta, " (*) usado\n");
      else
        fprintf(sta, "\n");
      fprintf(sta, "\toffset=%u\n", o = obj[n].tloc.offset);
      fprintf(sta, "\tlen1=%u\n", obj[n].tloc.len1);
      fprintf(sta, "\tlen2=%u\n", obj[n].tloc.len2);
      fprintf(sta, "\tlen3=%u\n", obj[n].tloc.len3);
      fprintf(sta, "\ttotalen=%u\n", obj[n].tloc.totalen);
      if (obj[n].member)
        fprintf(sta, "\tmember of %s\n", (*obj[n].member).name);
      break;
    case tsloc:
      fprintf(sta, "%5u\ttsloc: %s", n, obj[n].name);
      if (obj[n].usado)
        fprintf(sta, " (*) usado\n");
      else
        fprintf(sta, "\n");
      fprintf(sta, "\toffset=%u\n", o = obj[n].sloc.offset);
      fprintf(sta, "\titems1=%u\n", obj[n].sloc.items1);
      fprintf(sta, "\titems2=%u\n", obj[n].sloc.items2);
      fprintf(sta, "\titems3=%u\n", obj[n].sloc.items3);
      fprintf(sta, "\ttotalitems=%u\n", l = obj[n].sloc.totalitems);
      fprintf(sta, "\tlen_item=%u\n", obj[n].sloc.len_item);
      if (obj[n].member)
        fprintf(sta, "\tmember of %s\n", (*obj[n].member).name);
      break;
    case tpslo:
      fprintf(sta, "%5u\ttpslo: %s", n, obj[n].name);
      if (obj[n].usado)
        fprintf(sta, " (*) usado\n");
      else
        fprintf(sta, "\n");
      fprintf(sta, "\toffset=%u\n", o = obj[n].sloc.offset);
      fprintf(sta, "\titems1=%u\n", obj[n].sloc.items1);
      fprintf(sta, "\titems2=%u\n", obj[n].sloc.items2);
      fprintf(sta, "\titems3=%u\n", obj[n].sloc.items3);
      fprintf(sta, "\ttotalitems=%u\n", l = obj[n].sloc.totalitems);
      fprintf(sta, "\tlen_item=%u\n", obj[n].sloc.len_item);
      if (obj[n].member)
        fprintf(sta, "\tmember of %s\n", (*obj[n].member).name);
      break;
    case tproc:
      fprintf(sta, "%5u\ttproc: %s", n, obj[n].name);
      if (obj[n].usado)
        fprintf(sta, " (*) usado\n");
      else
        fprintf(sta, "\n");
      fprintf(sta, "\ttipo=%u\n", (int)(obj[n].proc.bloque));
      fprintf(sta, "\toffset=%u\n", obj[n].proc.offset);
      fprintf(sta, "\tnum_par=%u\n", obj[n].proc.num_par);
      break;
    case tfext:
      fprintf(sta, "%5u\ttfext: %s", n, obj[n].name);
      if (obj[n].usado)
        fprintf(sta, " (*) usado\n");
      else
        fprintf(sta, "\n");
      fprintf(sta, "\tcodigo=%u\n", obj[n].fext.codigo);
      fprintf(sta, "\tnum_par=%u\n", obj[n].fext.num_par);
      break;
    case tbglo:
      fprintf(sta, "%5u\ttbglo: %s", n, obj[n].name);
      if (obj[n].usado)
        fprintf(sta, " (*) usado\n");
      else
        fprintf(sta, "\n");
      fprintf(sta, "\toffset=%u\n", o = obj[n].bglo.offset);
      fprintf(sta, "\tlen1=%u\n", obj[n].bglo.len1);
      fprintf(sta, "\tlen2=%u\n", obj[n].bglo.len2);
      fprintf(sta, "\tlen3=%u\n", obj[n].bglo.len3);
      fprintf(sta, "\ttotalen=%u\n", obj[n].bglo.totalen);
      if (obj[n].member)
        fprintf(sta, "\tmember of %s\n", (*obj[n].member).name);
      break;
    case tbloc:
      fprintf(sta, "%5u\ttbloc: %s", n, obj[n].name);
      if (obj[n].usado)
        fprintf(sta, " (*) usado\n");
      else
        fprintf(sta, "\n");
      fprintf(sta, "\toffset=%u\n", o = obj[n].bglo.offset);
      fprintf(sta, "\tlen1=%u\n", obj[n].bglo.len1);
      fprintf(sta, "\tlen2=%u\n", obj[n].bglo.len2);
      fprintf(sta, "\tlen3=%u\n", obj[n].bglo.len3);
      fprintf(sta, "\ttotalen=%u\n", obj[n].bglo.totalen);
      if (obj[n].member)
        fprintf(sta, "\tmember of %s\n", (*obj[n].member).name);
      break;
    case twglo:
      fprintf(sta, "%5u\ttwglo: %s", n, obj[n].name);
      if (obj[n].usado)
        fprintf(sta, " (*) usado\n");
      else
        fprintf(sta, "\n");
      fprintf(sta, "\toffset=%u\n", o = obj[n].bglo.offset);
      fprintf(sta, "\tlen1=%u\n", obj[n].bglo.len1);
      fprintf(sta, "\tlen2=%u\n", obj[n].bglo.len2);
      fprintf(sta, "\tlen3=%u\n", obj[n].bglo.len3);
      fprintf(sta, "\ttotalen=%u\n", obj[n].bglo.totalen);
      if (obj[n].member)
        fprintf(sta, "\tmember of %s\n", (*obj[n].member).name);
      break;
    case twloc:
      fprintf(sta, "%5u\ttwloc: %s", n, obj[n].name);
      if (obj[n].usado)
        fprintf(sta, " (*) usado\n");
      else
        fprintf(sta, "\n");
      fprintf(sta, "\toffset=%u\n", o = obj[n].bglo.offset);
      fprintf(sta, "\tlen1=%u\n", obj[n].bglo.len1);
      fprintf(sta, "\tlen2=%u\n", obj[n].bglo.len2);
      fprintf(sta, "\tlen3=%u\n", obj[n].bglo.len3);
      fprintf(sta, "\ttotalen=%u\n", obj[n].bglo.totalen);
      if (obj[n].member)
        fprintf(sta, "\tmember of %s\n", (*obj[n].member).name);
      break;
    }

    char *msg_38 = get_translate(38, (unsigned int)obj[n].bloque, (unsigned int)obj[n].anterior);
    fprintf(sta, msg_38);
    free(msg_38);
    fprintf(sta, "\n");
  }

  fclose(sta);
}

/*
 *  Graba el fichero ensamblador
 */
void listado_ensamblador(void)
{
  FILE *sta;
  int i = mem[1];
  char nombreeml[256];

  strcpy(nombreeml, nombreprog);
  strcat(nombreeml, ".eml");

  if ((sta = fopen(nombreeml, "w")) == NULL)
  {
    print_translate(36, nombreeml);
    exit(1);
  }

  char *msg_39 = get_translate(39, fichero_prg);
  fprintf(sta, msg_39);
  free(msg_39);

  while (i < imem)
  {
    switch ((byte)mem[i])
    {
    case lnop:
      fprintf(sta, "%5u\tnop", i);
      break;
    case lcar:
      fprintf(sta, "%5u\tcar %u", i, mem[i + 1]);
      i++;
      break;
    case lasi:
      fprintf(sta, "%5u\tasi", i);
      break;
    case lori:
      fprintf(sta, "%5u\tori", i);
      break;
    case lxor:
      fprintf(sta, "%5u\txor", i);
      break;
    case land:
      fprintf(sta, "%5u\tand", i);
      break;
    case ligu:
      fprintf(sta, "%5u\tigu", i);
      break;
    case ldis:
      fprintf(sta, "%5u\tdis", i);
      break;
    case lmay:
      fprintf(sta, "%5u\tmay", i);
      break;
    case lmen:
      fprintf(sta, "%5u\tmen", i);
      break;
    case lmei:
      fprintf(sta, "%5u\tmei", i);
      break;
    case lmai:
      fprintf(sta, "%5u\tmai", i);
      break;
    case ladd:
      fprintf(sta, "%5u\tadd", i);
      break;
    case lsub:
      fprintf(sta, "%5u\tsub", i);
      break;
    case lmul:
      fprintf(sta, "%5u\tmul", i);
      break;
    case ldiv:
      fprintf(sta, "%5u\tdiv", i);
      break;
    case lmod:
      fprintf(sta, "%5u\tmod", i);
      break;
    case lneg:
      fprintf(sta, "%5u\tneg", i);
      break;
    case lptr:
      fprintf(sta, "%5u\tptr", i);
      break;
    case lnot:
      fprintf(sta, "%5u\tnot", i);
      break;
    case laid:
      fprintf(sta, "%5u\taid", i);
      break;
    case lcid:
      fprintf(sta, "%5u\tcid", i);
      break;
    case lrng:
      fprintf(sta, "%5u\trng %u", i, mem[i + 1]);
      i++;
      break;
    case ljmp:
      fprintf(sta, "%5u\tjmp %u", i, mem[i + 1]);
      i++;
      break;
    case ljpf:
      fprintf(sta, "%5u\tjpf %u", i, mem[i + 1]);
      i++;
      break;
    case lfun:
      fprintf(sta, "%5u\tfun %u", i, mem[i + 1]);
      i++;
      break;
    case lcal:
      fprintf(sta, "%5u\tcal %u", i, mem[i + 1]);
      i++;
      break;
    case lret:
      fprintf(sta, "%5u\tret", i);
      break;
    case lasp:
      fprintf(sta, "%5u\tasp", i);
      break;
    case lfrm:
      fprintf(sta, "%5u\tfrm", i);
      break;
    case lcbp:
      fprintf(sta, "%5u\tcbp %u", i, mem[i + 1]);
      i++;
      break;
    case lcpa:
      fprintf(sta, "%5u\tcpa", i);
      break;
    case ltyp:
      fprintf(sta, "\n%5u\ttyp %u", i, mem[i + 1]);
      i++;
      break;
    case lpri:
      fprintf(sta, "%5u\tpri %u", i, mem[i + 1]);
      i++;
      break;
    case lcse:
      fprintf(sta, "%5u\tcse %u", i, mem[i + 1]);
      i++;
      break;
    case lcsr:
      fprintf(sta, "%5u\tcsr %u", i, mem[i + 1]);
      i++;
      break;
    case lshr:
      fprintf(sta, "%5u\tshr", i);
      break;
    case lshl:
      fprintf(sta, "%5u\tshl", i);
      break;
    case lipt:
      fprintf(sta, "%5u\tipt", i);
      break;
    case lpti:
      fprintf(sta, "%5u\tpti", i);
      break;
    case ldpt:
      fprintf(sta, "%5u\tdpt", i);
      break;
    case lptd:
      fprintf(sta, "%5u\tptd", i);
      break;
    case lada:
      fprintf(sta, "%5u\tada", i);
      break;
    case lsua:
      fprintf(sta, "%5u\tsua", i);
      break;
    case lmua:
      fprintf(sta, "%5u\tmua", i);
      break;
    case ldia:
      fprintf(sta, "%5u\tdia", i);
      break;
    case lmoa:
      fprintf(sta, "%5u\tmoa", i);
      break;
    case lana:
      fprintf(sta, "%5u\tana", i);
      break;
    case lora:
      fprintf(sta, "%5u\tora", i);
      break;
    case lxoa:
      fprintf(sta, "%5u\txoa", i);
      break;
    case lsra:
      fprintf(sta, "%5u\tsra", i);
      break;
    case lsla:
      fprintf(sta, "%5u\tsla", i);
      break;
    case lpar:
      fprintf(sta, "%5u\tpar %u", i, mem[i + 1]);
      i++;
      break;
    case lrtf:
      fprintf(sta, "%5u\trtf", i);
      break;
    case lclo:
      fprintf(sta, "%5u\tclo %u", i, mem[i + 1]);
      i++;
      break;
    case lfrf:
      fprintf(sta, "%5u\tfrf", i);
      break;
    case limp:
      fprintf(sta, "%5u\timp %u", i, mem[i + 1]);
      i++;
      break;
    case lext:
      fprintf(sta, "%5u\text %u", i, mem[i + 1]);
      i++;
      break;
    case lchk:
      fprintf(sta, "%5u\tchk", i);
      break;
    case ldbg:
      fprintf(sta, "%5u\tdbg", i);
      break;

    case lcar2:
      fprintf(sta, "%5u\tcar2 %u %u", i, mem[i + 1], mem[i + 2]);
      i += 2;
      break;
    case lcar3:
      fprintf(sta, "%5u\tcar3 %u %u %u", i, mem[i + 1], mem[i + 2], mem[i + 3]);
      i += 3;
      break;
    case lcar4:
      fprintf(sta, "%5u\tcar4 %u %u %u %u", i, mem[i + 1], mem[i + 2], mem[i + 3], mem[i + 4]);
      i += 4;
      break;
    case lasiasp:
      fprintf(sta, "%5u\tasiasp", i);
      break;
    case lcaraid:
      fprintf(sta, "%5u\tcaraid %u", i, mem[i + 1]);
      i++;
      break;
    case lcarptr:
      fprintf(sta, "%5u\tcarptr %u", i, mem[i + 1]);
      i++;
      break;
    case laidptr:
      fprintf(sta, "%5u\taidptr", i);
      break;
    case lcaraidptr:
      fprintf(sta, "%5u\tcaraidptr %u", i, mem[i + 1]);
      i++;
      break;
    case lcaraidcpa:
      fprintf(sta, "%5u\tcaraidcpa %u", i, mem[i + 1]);
      i++;
      break;
    case laddptr:
      fprintf(sta, "%5u\taddptr", i);
      break;
    case lfunasp:
      fprintf(sta, "%5u\tfunasp %u", i, mem[i + 1]);
      i++;
      break;
    case lcaradd:
      fprintf(sta, "%5u\tcaradd %u", i, mem[i + 1]);
      i++;
      break;
    case lcaraddptr:
      fprintf(sta, "%5u\tcaraddptr %u", i, mem[i + 1]);
      i++;
      break;
    case lcarmul:
      fprintf(sta, "%5u\tcarmul %u", i, mem[i + 1]);
      i++;
      break;
    case lcarmuladd:
      fprintf(sta, "%5u\tcarmuladd %u", i, mem[i + 1]);
      i++;
      break;
    case lcarasiasp:
      fprintf(sta, "%5u\tcarasiasp %u", i, mem[i + 1]);
      i++;
      break;
    case lcarsub:
      fprintf(sta, "%5u\tcarsub %u", i, mem[i + 1]);
      i++;
      break;
    case lcardiv:
      fprintf(sta, "%5u\tcardiv %u", i, mem[i + 1]);
      i++;
      break;

    case lptrwor:
      fprintf(sta, "%5u\tptrwor", i);
      break;
    case lasiwor:
      fprintf(sta, "%5u\tasiwor", i);
      break;
    case liptwor:
      fprintf(sta, "%5u\tiptwor", i);
      break;
    case lptiwor:
      fprintf(sta, "%5u\tptiwor", i);
      break;
    case ldptwor:
      fprintf(sta, "%5u\tdptwor", i);
      break;
    case lptdwor:
      fprintf(sta, "%5u\tptdwor", i);
      break;
    case ladawor:
      fprintf(sta, "%5u\tadawor", i);
      break;
    case lsuawor:
      fprintf(sta, "%5u\tsuawor", i);
      break;
    case lmuawor:
      fprintf(sta, "%5u\tmuawor", i);
      break;
    case ldiawor:
      fprintf(sta, "%5u\tdiawor", i);
      break;
    case lmoawor:
      fprintf(sta, "%5u\tmoawor", i);
      break;
    case lanawor:
      fprintf(sta, "%5u\tanawor", i);
      break;
    case lorawor:
      fprintf(sta, "%5u\torawor", i);
      break;
    case lxoawor:
      fprintf(sta, "%5u\txoawor", i);
      break;
    case lsrawor:
      fprintf(sta, "%5u\tsrawor", i);
      break;
    case lslawor:
      fprintf(sta, "%5u\tslawor", i);
      break;
    case lcpawor:
      fprintf(sta, "%5u\tcpawor", i);
      break;

    case lptrchr:
      fprintf(sta, "%5u\tptrchr", i);
      break;
    case lasichr:
      fprintf(sta, "%5u\tasichr", i);
      break;
    case liptchr:
      fprintf(sta, "%5u\tiptchr", i);
      break;
    case lptichr:
      fprintf(sta, "%5u\tptichr", i);
      break;
    case ldptchr:
      fprintf(sta, "%5u\tdptchr", i);
      break;
    case lptdchr:
      fprintf(sta, "%5u\tptdchr", i);
      break;
    case ladachr:
      fprintf(sta, "%5u\tadachr", i);
      break;
    case lsuachr:
      fprintf(sta, "%5u\tsuachr", i);
      break;
    case lmuachr:
      fprintf(sta, "%5u\tmuachr", i);
      break;
    case ldiachr:
      fprintf(sta, "%5u\tdiachr", i);
      break;
    case lmoachr:
      fprintf(sta, "%5u\tmoachr", i);
      break;
    case lanachr:
      fprintf(sta, "%5u\tanachr", i);
      break;
    case lorachr:
      fprintf(sta, "%5u\torachr", i);
      break;
    case lxoachr:
      fprintf(sta, "%5u\txoachr", i);
      break;
    case lsrachr:
      fprintf(sta, "%5u\tsrachr", i);
      break;
    case lslachr:
      fprintf(sta, "%5u\tslachr", i);
      break;
    case lcpachr:
      fprintf(sta, "%5u\tcpachr", i);
      break;

    case lstrcpy:
      fprintf(sta, "%5u\tstrcpy", i);
      break;
    case lstrfix:
      fprintf(sta, "%5u\tstrfix", i);
      break;
    case lstrcat:
      fprintf(sta, "%5u\tstrcat", i);
      break;
    case lstradd:
      fprintf(sta, "%5u\tstradd", i);
      break;
    case lstrdec:
      fprintf(sta, "%5u\tstrdec", i);
      break;
    case lstrsub:
      fprintf(sta, "%5u\tstrsub", i);
      break;
    case lstrlen:
      fprintf(sta, "%5u\tstrlen", i);
      break;
    case lstrigu:
      fprintf(sta, "%5u\tstrigu", i);
      break;
    case lstrdis:
      fprintf(sta, "%5u\tstrdis", i);
      break;
    case lstrmay:
      fprintf(sta, "%5u\tstrmay", i);
      break;
    case lstrmen:
      fprintf(sta, "%5u\tstrmen", i);
      break;
    case lstrmei:
      fprintf(sta, "%5u\tstrmei", i);
      break;
    case lstrmai:
      fprintf(sta, "%5u\tstrmai", i);
      break;
    case lcpastr:
      fprintf(sta, "%5u\tcpastr", i);
      break;

    case lnul:
      fprintf(sta, "%5u\tnul", i);
      break;

    case lextasp:
      fprintf(sta, "%5u\textasp %u", i, mem[i + 1]);
      i++;
      break;

    default:
      fprintf(sta, "***");
      break;
    }
    fprintf(sta, "\n");
    i++;
  }
  fclose(sta);
}
