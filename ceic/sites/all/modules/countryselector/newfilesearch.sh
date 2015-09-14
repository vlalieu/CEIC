#!/bin/bash
find -L /var/www/kic/desktop/ -type f -print -name "*"  | awk ' 
BEGIN {
	S = "0";
	C = "0";
	D = "0";
	if ((length("'$3'") > 0) && (index("nixberniks","'$3'") == 0))	{
		SSTR = tolower("'$3'");
		S = "1";
		}
	if (length("'$2'") > 0) 	{
		C = "1";
		}
	if (length("'$4'") > 0) 	{
		D = "1";
		}
	SCD = S C D;
}
function tronculate(str,c,len, res) {
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
{
	YY=tolower($0);
	YZ=tronculate(YY);
        OK = 0;
	    switch (SCD) {
	     case "100":
	       if (index(YZ, SSTR) > 0) OK=1;
		break;
	     case "110":
		if ((index(YZ, SSTR) > 0) && (index($0, "'$2'") > 0)) OK=1;	       
		break;
	     case "101":
		if ((index(YZ, SSTR) > 0) && (index($0, "'$4'") > 0)) OK=1;	       
		break;
	     case "111":
		if ((index(YZ, SSTR) > 0) && (index($0, "'$2'") > 0) && (index($0, "'$4'") > 0)) OK = 1;	       
		break;
	     case "011":
		if ((index($0, "'$2'") > 0) && (index($0, "'$4'") > 0)) OK = 1;			       
		break;
	     default:
		break;
	     }
	if (OK > 0) print $0;
	} ' | while read fn; do ls -lsa "${fn}" | awk ' 
BEGIN { 
	FS=" "; 
	for (i = 0; i <= 255; i++) ord[sprintf("%c", i)] = i;
	} 


function escape(str, c, len, res) {
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
	ISLINK=0;
	ISLINK = index($2, "'$1'");
	if (ISLINK == 1) {
		KK0 = "";
		for (K=10;K<=NF;K++) {
			if (K>10) KK0=KK0 " ";
			KK0 = KK0 $K;
			}
		PT1 = split(KK0, KK1, ".");
		SP1 = split(KK1[2], KK2, " ");
		KK3 = KK1[1] "." KK2[1];
		XU = split(KK3, UU, "/");
		ZZ = "http://www.kaspinfo.com/kic/desktop";
		UR = "";
		for (UI in UU)	{
			 I++;
			 if ((I < XU) && (I>5)) 	{
				ZZ = ZZ "/" UU[I];
				UR = UR "/" UU[I];
				}
			}
		UT=escape(UU[XU]);
		ZZ = ZZ "/" UT;
		sub("/var/www/kic/desktop/","",$10);
		
		printf("<tr><td>%s</td><td><a href=%s>%s</a></td></tr>\n",UR,ZZ,UU[XU]);

               }
	}' 
done | awk ' BEGIN { 
	FS=">";
	XX = "";
	}
function tronculate(str,c,len, res) {
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

{
	split($6, AA, ">");
	YY=tolower(AA[1]);
	YZ=tronculate(YY);
	if (index(XX, YZ) == 0)	{
		print $0;
		XX = XX " # " YZ;
	}
}
END { 
	} ' 


