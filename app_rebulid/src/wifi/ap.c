/*
* AP
*/
#include "ap.h"
#include "base/ja_process.h"
//open udhcpd
void AP_openUdhcpd(char *fileName)
{
	char str[700];
	FILE *fb;
	
	memset(str, '\0', sizeof(str));
	
	sprintf(str, "udhcpd %s -S &", fileName);
	NK_SYSTEM(str);
	
	/*fb= popen(str, "r");
    if(fb==NULL)
	{ 
		printf("popen error: %s/n", strerror(errno)); 	
	} */  
}

//set udhcpd configure
void AP_setUdhcpd(struct ap_struct *tsap, char *interface, char *fileName)
{
	FILE *fp;
	char str[500];
	char ipEnd[20];
	int a,b,c,d,m;
	
	sscanf(tsap->ipStart, "%d.%d.%d.%d", &a,&b,&c,&d);
	m = atoi(tsap->ipNumber);
	d = d+m;
	sprintf(ipEnd,"%d.%d.%d.%d",a,b,c,d);
	
	fp = fopen(fileName, "w+");
	if(fp==NULL)
	{ 
		printf("fopen %s error: %s/n", fileName, strerror(errno)); 	
		return;
	}
	
	//output change data
	strcpy(str, "option domain local\n");
	strcat(str, "option lease 1800\n");
	fputs(str, fp);
	fprintf(fp, "max_leases %d\nstart %s\nend %s\ninterface %s\nopt dns %s\noption subnet %s\nopt router %s\n", m+1, tsap->ipStart, ipEnd, interface, tsap->dns, tsap->subnet, tsap->router);	
	
	fclose(fp);
}

//open AP (hostapd)
void AP_openAP(char *fileName)
{
	char str[700];
	FILE *fb;
	
	memset(str, '\0', sizeof(str));
	
	sprintf(str, "/usr/share/ipcam/wifi_tools/hostapd %s -B", fileName);
	NK_SYSTEM(str);
	/*fb= popen(str, "r");
    if(fb==NULL)
	{ 
		printf("popen error: %s/n", strerror(errno)); 	
	}*/ 
}

//set the AP configuration
void AP_setHostapd(struct ap_struct *tsap, char *fileName)
{
	FILE *fp;
	char str[1024] = "";
	
	fp = fopen(fileName, "w+");
	if(fp==NULL)
	{ 
		printf("fopen %s error: %s/n", fileName, strerror(errno)); 	
		return ;
	}

	if(strlen(tsap->wpa_passphrase) == 0){
		snprintf(str, sizeof(str), "interface=wlan0\n"		//接入点设备名
			"ctrl_interface=/tmp/hostapd\n"
			"wmm_enabled=1\n"								//wmm(无线多媒体)，开启它， 音视频媒体优先播放：1 = 打开，0关闭
			"macaddr_acl=0\n"								//可选，指定MAC地址过滤规则，0表示除非在禁止列表否则允许，1表示除非在允许列表否则禁止，2表示使用外部RADIUS服务器；
			"auth_algs=1\n"									//认证方式
			"ignore_broadcast_ssid=0\n" ·					//打开或隐藏无线热点：1 = 隐藏 ，0 = 不隐藏
			"ssid=%s\n"										//别人所看到的我们这个无线接入点的名称
			"channel=%s\n", 								//无线频道
			tsap->essid, tsap->channel);
		fputs(str, fp);
		if(!strcmp(tsap->hw_mode,"bgn"))
		{
			fputs("hw_mode=g\n", fp);	
			fputs("ieee80211n=1\n", fp);	
		}else if(!strcmp(tsap->hw_mode,"bg"))
		{
			fputs("hw_mode=g\n", fp);	
			fputs("ieee80211n=0\n", fp);	
		}else if(!strcmp(tsap->hw_mode,"g"))
		{
			fprintf(fp, "hw_mode=%s\n", "g");	
		}else{	
			fputs("hw_mode=g\n", fp);	      //bgn  ----- bug
			fputs("ieee80211n=1\n", fp);	 
		}
		
	}else{
		//output fixation data
		strcpy(str, "interface=wlan0\n");
		strcat(str, "ctrl_interface=/tmp/hostapd\n");
		//strcat(str, "bridge=br0\n");
		strcat(str, "wmm_enabled=1\n");
		//strcat(str, "ieee80211d=1\n");
		strcat(str, "ht_capab=[SHORT-GI-20]\n");
		strcat(str, "macaddr_acl=0\n");
		strcat(str, "auth_algs=1\n");
		strcat(str, "ignore_broadcast_ssid=0\n");
		strcat(str, "wpa_pairwise=TKIP\n");
		strcat(str, "rsn_pairwise=CCMP\n");
		
		//strcat(str, "ht_capab=[HT40-][SHORT-GI-20][SHORT-GI-40]\n");
		//strcat(str, "require_ht=1\n");
		fputs(str, fp);
		
		//output change data
		fprintf(fp, "ssid=%s\nwpa_passphrase=%s\nchannel=%s\n", tsap->essid, tsap->wpa_passphrase, tsap->channel);
		if(!strcmp(tsap->hw_mode,"bgn"))
		{
			fputs("hw_mode=g\n", fp);	
			fputs("ieee80211n=1\n", fp);	
		}else if(!strcmp(tsap->hw_mode,"bg"))
		{
			fputs("hw_mode=g\n", fp);	
			fputs("ieee80211n=0\n", fp);	
		}else if(!strcmp(tsap->hw_mode,"g"))
		{
			fprintf(fp, "hw_mode=%s\n", "g");	
		}else{	
			fputs("hw_mode=g\n", fp);	      //bgn  ----- bug
			fputs("ieee80211n=1\n", fp);	 
		}
		
		//wpa-psk   wpa2-psk
		if(!strcmp(tsap->wpa_key_mgmt,"WPA-PSK"))
		{	
			fputs("wpa=0\nwpa_key_mgmt=WPA-PSK\n", fp);
		}else if(!strcmp(tsap->wpa_key_mgmt,"WPA2-PSK"))
		{
			fputs("wpa=2\nwpa_key_mgmt=WPA-PSK\n", fp);
		}
	}
	fclose(fp);
}


/*
int main(void)
{
	AP_setHostapd("goodlucky22","goodlucky","g","WPA-PSK","11","ap.conf"); //set the AP configuration
	
	AP_openAP("ap.conf");   //open AP (hostapd)
	
	AP_setUdhcpd("192.168.2.200", "20", "wlan0", "8.8.8.8", "255.255.255.0", "192.168.2.1", "udhcpd.conf");  //set udhcpd configure
	
	AP_openUdhcpd("udhcpd.conf");   //open udhcpd
	
	return 0;
}
*/
