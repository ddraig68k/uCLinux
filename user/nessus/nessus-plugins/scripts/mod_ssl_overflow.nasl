#
# This script was written by Renaud Deraison <deraison@cvs.nessus.org>,
# with the impulsion of H D Moore on the Nessus Plugins-Writers list
#
# See the Nessus Scripts License for details
#

if(description)
{
 script_id(10888);
 script_cve_id("CVE-2002-0082");
 script_bugtraq_id(4189);
 script_version("$Revision: 1.12 $");
 
 name["english"] = "mod_ssl overflow";

 script_name(english:name["english"]);
 
 desc["english"] = "
The remote host is using a version of mod_ssl which is
older than 2.8.7.

This version is vulnerable to a buffer overflow which,
albeit difficult to exploit, may allow an attacker
to obtain a shell on this host.

*** Some vendors patched older versions of mod_ssl, so this
*** might be a false positive. Check with your vendor to determine
*** if you have a version of mod_ssl that is patched for this 
*** vulnerability


Solution : Upgrade to version 2.8.7 or newer
Risk factor : High";

 script_description(english:desc["english"], francais:desc["francais"]);
 
 summary["english"] = "Checks for version of mod_ssl";
 summary["francais"] = "V?rifie la version de mod_ssl";
 
 script_summary(english:summary["english"], francais:summary["francais"]);
 
 script_category(ACT_GATHER_INFO);
 
 
 script_copyright(english:"This script is Copyright (C) 2002 Renaud Deraison",
		francais:"Ce script est Copyright (C) 2002 Renaud Deraison");
 family["english"] = "CGI abuses";
 family["francais"] = "Abus de CGI";
 script_family(english:family["english"], francais:family["francais"]);
 script_dependencie("find_service.nes", "no404.nasl", "http_version.nasl");
 script_require_ports("Services/www", 80);
 script_require_keys("www/apache");
 exit(0);
}

#
# The script code starts here
#
include("http_func.inc");

port = get_kb_item("Services/www");
if(!port)port = 80;
if(get_port_state(port))
{
 banner = get_http_banner(port:port);
 if(!banner)exit(0);

 serv = strstr(banner, "Server");
 if("Apache/2" >< serv) exit(0);
 if("Apache-AdvancedExtranetServer/2" >< serv)exit(0);

 if(ereg(pattern:".*mod_ssl/(1.*|2\.([0-7]\..*|8\.[0-6][^0-9])).*", string:serv))
 {
   security_hole(port);
 }
}
