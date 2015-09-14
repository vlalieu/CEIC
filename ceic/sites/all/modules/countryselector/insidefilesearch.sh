#!/bin/bash
function pyexec() {
echo "$(/usr/bin/python -c 'import sys, urllib as ul; print ul.unquote_plus(sys.stdin.read())')"    
}


dtronc="";
if [ !  -z "$3" ] && [ "$3" != "nixberniks" ]
 then
	dtronc=$(echo $3 | pyexec);
#	echo $dtronc;
 fi
 if [ -z "$4" ]
	then 
		cmdstr="find -L /var/www/kic/desktop/$2/";
	else
		cmdstr="find -L /var/www/kic/desktop/$2/sections/$4/";
 fi
 
$cmdstr -name "*" -type f -print   | awk ' 
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
BEGIN {
	XX = "";
	S = "0";
	C = "1";
	D = "0";
	if ((length("'$3'") > 0) && (index("nixberniks","'$3'") == 0))	{
		SSTR = tolower("'$3'");
		S = "1";
		}
	if (length("'$4'") > 0) 	{
		D = "1";
		}
	SCD = S C D;
}
{
	YY=tolower($0);
	YZ=tronculate(YY);
        OK = 0;
	if (index($0, "'$2'") > 0) OK=1;	       
	if (index($0, "'$4'") > 0) OK=1;	       
	if (OK > 0) 	{
		if (index(XX, YZ) == 0)	{
			print $0;
			XX = XX " # " YZ;
			}
		}

	} ' | while read fn; do 
	OK=0;
	if  [ ${#dtronc} -gt 0 ]
	then
		FILDOC=$(echo "${fn}" | awk '/.doc/ {print $0;}');
		if [ ! -z "$FILDOC" ]
		  then
#		      echo "my piece of search text is " $dtronc;
	  	      AA=$(catdoc -scp1252 "${fn}" | grep -c "$dtronc");
		      if [ "$AA" -gt "0" ] 
			then
	#		 echo "match doc ${fn}";
			 OK=1;
			 Match="inside_doc";
			fi
		  fi
		MatchFN=$(echo "${fn}" | awk '/'$3'/ {print $0;}');
		if [ ! -z "$MatchFN" ] 
		then
			OK=1;
			Match="in_title";
	#		echo "match title ${fn}";
		fi	
	else
		OK=1;
	fi
	if [ "$OK" -gt "0" ]
	then
ls -lsa "${fn}" | awk ' 
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
	IsCaspeco=0;
	IsCaspeco = index($2, "'$1'");
	if (IsCaspeco == 1) {
		li1 = index($0, "->"); 
		if (li1 > 0) {
			aa1 = split($0, aa, " -> ");
			u1 = split(aa[1],uu,":");
			nom_fichier=substr(uu[2], u1+4, length(uu[2])-5);
			split(aa[2],nom_lien,"ASTANA-UNDP-");
				XU = split(nom_fichier, UU, "/");
				ZZ = "http://www.kaspinfo.com/kic/desktop";
				repertoire = "";
				for (UI in UU)	{
					 I++;
					 if ((I < XU) && (I>4)) 	{
						ZZ = ZZ "/" UU[I];
						repertoire = repertoire "/" UU[I];
						}
					}
				UT=escape(UU[XU]);
				ZZ = ZZ "/" UT;
				sub("/var/www/kic/desktop/","",nom_fichier);
				printf("<tr><td>Match '$Match'</td><td>%s</td><td><a href=%s title=\"source: %s\">%s</a></td></tr>\n",repertoire,ZZ,nom_lien[2],UU[XU]);			
			}
		else	{
				u1 = split($0,uu,":");
				nom_fichier=substr(uu[2], u1+4, length(uu[2])-5);
				XU = split(nom_fichier, UU, "/");
				ZZ = "http://www.kaspinfo.com/kic/desktop";
				repertoire = "";
				for (UI in UU)	{
					 I++;
					 if ((I < XU) && (I>5)) 	{
						ZZ = ZZ "/" UU[I];
						repertoire = repertoire "/" UU[I];
						}
					}
				UT=escape(UU[XU]);
				ZZ = ZZ "/" UT;
				sub("/var/www/kic/desktop/","",nom_fichier);
				printf("<tr><td>Match '$Match'</td><td>%s</td><td><a href=%s >%s</a></td></tr>\n",ZZ,nom_lien[2],UU[XU]);
			}
               }
	}' 		
	fi

done 

