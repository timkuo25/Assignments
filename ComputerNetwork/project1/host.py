import socket
import urllib.parse
import requests
from bs4 import BeautifulSoup
import threading

#first argument for address(IP version 4), second for socket type(TCP protocol)
IRCSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
IRCSocket.connect(("140.112.28.129", 6667))

Msg = "NICK bot_b05703100 \r\n"
IRCSocket.send(bytes(Msg, encoding = "utf-8"))

Msg = "USER b05703100 \r\n"
IRCSocket.send(bytes(Msg, encoding = "utf-8"))

Msg = "JOIN #CN_DEMO \r\n"
IRCSocket.send(bytes(Msg, encoding = "utf-8"))

Msg = "PRIVMSG #CN_DEMO :Hi, I'm b05703100 \r\n"
IRCSocket.send(bytes(Msg, encoding = "utf-8"))

def talk(str1):
	print("talk initiated")
	while var == 1:
		msg = input()
		Msg = "PRIVMSG " + str1 + " :" + msg + " \r\n"
		IRCSocket.send(bytes(Msg, encoding = "utf-8"))

def recv():
	global var
	print("recv initiated")
	while True:
		IRCMsg = IRCSocket.recv(4096).decode()
		if IRCMsg == "PING localhost :localhost":
			continue
		test7 = IRCMsg.split()
		print(IRCMsg)
		if test7[3] == ":!bye":
			var = 0
			break
	
while True:
	var = 1
	IRCMsg = IRCSocket.recv(4096).decode()
	print(IRCMsg)
	if IRCMsg == "PING localhost :localhost":
		continue
	test1 = IRCMsg.split() #test1判斷命令的種類
	if test1[1] == "PRIVMSG":
		#print("You've got a message")
		test2 = IRCMsg.split(":")
		test3 = test2[1].split("!")
		aite = test3[0]
		com = test1[3]
#星座運勢
		if com == ":Capricorn":
			Msg = "PRIVMSG " + aite + " :本日星座運勢>< \r\n"
			IRCSocket.send(bytes(Msg, encoding = "utf-8"))
		elif com == ":Aquarius":
			Msg = "PRIVMSG " + aite + " :本日星座運勢>< \r\n"
			IRCSocket.send(bytes(Msg, encoding = "utf-8"))
		elif com == ":Pisces":
			Msg = "PRIVMSG " + aite + " :本日星座運勢>< \r\n"
			IRCSocket.send(bytes(Msg, encoding = "utf-8"))
		elif com == ":Aries":
			Msg = "PRIVMSG " + aite + " :本日星座運勢>< \r\n"
			IRCSocket.send(bytes(Msg, encoding = "utf-8"))
		elif com == ":Taurus":
			Msg = "PRIVMSG " + aite + " :本日星座運勢>< \r\n"
			IRCSocket.send(bytes(Msg, encoding = "utf-8"))
		elif com == ":Gemini":
			Msg = "PRIVMSG " + aite + " :本日星座運勢>< \r\n"
			IRCSocket.send(bytes(Msg, encoding = "utf-8"))
		elif com == ":Cancer":
			Msg = "PRIVMSG " + aite + " :本日星座運勢>< \r\n"
			IRCSocket.send(bytes(Msg, encoding = "utf-8"))
		elif com == ":Leo":
			Msg = "PRIVMSG " + aite + " :本日星座運勢>< \r\n"
			IRCSocket.send(bytes(Msg, encoding = "utf-8"))
		elif com == ":Virgo":
			Msg = "PRIVMSG " + aite + " :本日星座運勢>< \r\n"
			IRCSocket.send(bytes(Msg, encoding = "utf-8"))
		elif com == ":Libra":
			Msg = "PRIVMSG " + aite + " :本日星座運勢>< \r\n"
			IRCSocket.send(bytes(Msg, encoding = "utf-8"))
		elif com == ":Scorpio":
			Msg = "PRIVMSG " + aite + " :本日星座運勢>< \r\n"
			IRCSocket.send(bytes(Msg, encoding = "utf-8"))
		elif com == ":Sagittarius":
			Msg = "PRIVMSG " + aite + " :本日星座運勢>< \r\n"
			IRCSocket.send(bytes(Msg, encoding = "utf-8"))

#猜數字
		elif com == ":!guess":
			numList = [1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
			ans = 7
			Msg = "PRIVMSG " + aite + " :猜一個1~10之間的數字 \r\n"
			IRCSocket.send(bytes(Msg, encoding = "utf-8"))
			while 1:
				IRCMsg = IRCSocket.recv(4096).decode()
				if IRCMsg == "PING localhost :localhost":
					continue
				test4 = IRCMsg.split()
				test5 = test4[3].split(":")
				num = test5[1]
				if num.isnumeric():
					num = int(num)
					if num > 10 or num < 1:
						Msg = "PRIVMSG " + aite + " :1到10= = \r\n"
						IRCSocket.send(bytes(Msg, encoding = "utf-8"))
						
					elif num == ans:
						Msg = "PRIVMSG " + aite + " :正確答案為" + str(ans) + "! 恭喜猜中\r\n"
						IRCSocket.send(bytes(Msg, encoding = "utf-8"))
						break
					elif not numList[num - 1]:
						if num > ans:
							Msg = "PRIVMSG " + aite + " :你猜過" + str(num) + "了= = 小於" + str(num) + "!\r\n"
							IRCSocket.send(bytes(Msg, encoding = "utf-8"))
						else:
							Msg = "PRIVMSG " + aite + " :你猜過" + str(num) + "了= = 大於" + str(num) + "!\r\n"
							IRCSocket.send(bytes(Msg, encoding = "utf-8"))
					elif num > ans:
						Msg = "PRIVMSG " + aite + " :小於" + str(num) + "!\r\n"
						IRCSocket.send(bytes(Msg, encoding = "utf-8"))
						numList[num - 1] = 0
					elif num < ans:
						Msg = "PRIVMSG " + aite + " :大於" + str(num) + "!\r\n"
						IRCSocket.send(bytes(Msg, encoding = "utf-8"))
						numList[num - 1] = 0
				else:
					Msg = "PRIVMSG " + aite + " :1到10= =\r\n"
					IRCSocket.send(bytes(Msg, encoding = "utf-8"))
#爬蟲
		elif com == ":!song":
			test6 = IRCMsg.split(":!song ")
			query = test6[1]
			code = urllib.parse.quote_plus(query, encoding = "utf-8")
			url = "http://www.youtube.com/results?search_query=" + code
			resp = requests.get(url)
			soup = BeautifulSoup(resp.text, "html.parser")
			a = soup.find_all('a')
			objective = ""
			for p in a:
				target = p.get('href').split("?")
				if target[0] == "/watch":
					objective = p.get('href')
					break
			url = "http://www.youtube.com" + objective
			Msg = "PRIVMSG " + aite + " :" + url + " \r\n"
			IRCSocket.send(bytes(Msg, encoding = "utf-8"))
#聊天
		elif com == ":!chat":
			Msg = "PRIVMSG " + aite + " :已取得聯繫 \r\n"
			IRCSocket.send(bytes(Msg, encoding = "utf-8"))
			
			print(aite + "想聯繫你")
			t1 = threading.Thread(target = recv)
			t2 = threading.Thread(target = talk, args=[aite])
			t1.start()
			t2.start()
			t1.join()
			t2.join()
						
		else:
			Msg = "PRIVMSG " + aite + " :How are You? \r\n"
			IRCSocket.send(bytes(Msg, encoding = "utf-8"))