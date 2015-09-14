#!/bin/bash
find -L '/var/www/kic/desktop/' -type f -mtime -25000 -printf %TY:%Tm\;%h\;%f\\n | awk ' 
BEGIN { 
	FS=";"; 
	S = "0";
	C = "0";
	D = "0";
	if ((length("'$1'") > 0) && (index("nixberniks","'$1'") == 0))	{
		SSTR = tolower("'$1'");
		S = "1";
		}
	if (length("'$2'") > 0) 	{
		C = "1";
		}
	if (length("'$3'") > 0) 	{
		D = "1";
		}
	SCD = S C D;

	XX = "";
	for (i = 0; i <= 255; i++) ord[sprintf("%c", i)] = i;
	} 

function tronculate(str,    c, len, res) {
	len = length(str)
	res = ""
	for (i = 1; i <= len; i++) {
		c = substr(str, i, 1);
		if (c == " ")
			res = res "+"
		else if (c == "_")
			res = res "+"
		else
			res = res c
	}
	return res
} 

function escape(str,    c, len, res) {
	len = length(str)
	res = ""
	for (i = 1; i <= len; i++) {
		c = substr(str, i, 1);
		if (c ~ /[0-9A-Za-z]/)
		#if (c ~ /[-._*0-9A-Za-z]/)
			res = res c
		#else if (c == " ")
		#	res = res "+"
		else
			res = res "%" sprintf("%02X", ord[c])
	}
	return res
} 
{
	YY=tolower($3);
	YZ=tronculate(YY);
        OK = 0;
	    switch (SCD) {
	     case "100":
	       if (index(YZ, SSTR) > 0) OK=1;
		break;
	     case "110":
		if ((index(YZ, SSTR) > 0) && (index($2, "'$2'") > 0)) OK=1;	       
		break;
	     case "101":
		if ((index(YZ, SSTR) > 0) && (index($2, "'$3'") > 0)) OK=1;	       
		break;
	     case "111":
		if ((index(YZ, SSTR) > 0) && (index($2, "'$2'") > 0) && (index($2, "'$3'") > 0)) OK = 1;	       
		break;
	     case "011":
		if ((index($2, "'$2'") > 0) && (index($2, "'$3'") > 0)) OK = 1;			       
		break;
	     default:
		break;
	     }
	if (OK > 0) {
		if (index(XX, $3) == 0)	{
			sub("/var/www","http://www.kaspinfo.com",$2);
			UU=$2;
			sub("http://www.kaspinfo.com/kic/desktop","",UU);
			ZZ=escape($3);
			printf("<tr><td>%s</td><td><a href=%s/%s>%s</a></td></tr>\n",UU,$2,ZZ,$3);
			XX = XX " # " $3;
			}
		}
	} 
END { 
	} ' 



