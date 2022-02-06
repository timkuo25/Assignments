import socket

def talk():
	msg = input()
	Msg = "PRIVMSG bot_b05703100 :" + msg + " \r\n"
	IRCSocket.send(bytes(Msg, encoding = "utf-8"))

IRCSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
IRCSocket.connect(("127.0.0.1", 6667))

Msg = "NICK test \r\n"
IRCSocket.send(bytes(Msg, encoding = "utf-8"))
IRCMsg = IRCSocket.recv(4096).decode()
print(IRCMsg)

Msg = "USER TA \r\n"
IRCSocket.send(bytes(Msg, encoding = "utf-8"))
IRCMsg = IRCSocket.recv(4096).decode()
print(IRCMsg)

Msg = "JOIN #CN_DEMO \r\n"
IRCSocket.send(bytes(Msg, encoding = "utf-8"))
IRCMsg = IRCSocket.recv(4096).decode()
print(IRCMsg)


while 1:
	IRCMsg = IRCSocket.recv(4096).decode()
	if IRCMsg == "PING localhost :localhost":
		continue
	print(IRCMsg)
	talk()
'''Msg = "PRIVMSG bot_b05703100 :!song you belong with me \r\n"
IRCSocket.send(bytes(Msg, encoding = "utf-8"))
IRCMsg = IRCSocket.recv(4096).decode()
print(IRCMsg)

Msg = "PRIVMSG bot_b05703100 :5 \r\n"
IRCSocket.send(bytes(Msg, encoding = "utf-8"))
IRCMsg = IRCSocket.recv(4096).decode()
print(IRCMsg)

Msg = "PRIVMSG bot_b05703100 :Libra \r\n"
IRCSocket.send(bytes(Msg, encoding = "utf-8"))
IRCMsg = IRCSocket.recv(4096).decode()
print(IRCMsg)

Msg = "PRIVMSG bot_b05703100 :!guess \r\n"
IRCSocket.send(bytes(Msg, encoding = "utf-8"))
IRCMsg = IRCSocket.recv(4096).decode()
print(IRCMsg)

Msg = "PRIVMSG bot_b05703100 :4 \r\n"
IRCSocket.send(bytes(Msg, encoding = "utf-8"))
IRCMsg = IRCSocket.recv(4096).decode()
print(IRCMsg)

Msg = "PRIVMSG bot_b05703100 :5 \r\n"
IRCSocket.send(bytes(Msg, encoding = "utf-8"))
IRCMsg = IRCSocket.recv(4096).decode()
print(IRCMsg)

Msg = "PRIVMSG bot_b05703100 :Libra \r\n"
IRCSocket.send(bytes(Msg, encoding = "utf-8"))
IRCMsg = IRCSocket.recv(4096).decode()
print(IRCMsg)

Msg = "PRIVMSG bot_b05703100 :4 \r\n"
IRCSocket.send(bytes(Msg, encoding = "utf-8"))
IRCMsg = IRCSocket.recv(4096).decode()
print(IRCMsg)

Msg = "PRIVMSG bot_b05703100 :10 \r\n"
IRCSocket.send(bytes(Msg, encoding = "utf-8"))
IRCMsg = IRCSocket.recv(4096).decode()
print(IRCMsg)

Msg = "PRIVMSG bot_b05703100 :7 \r\n"
IRCSocket.send(bytes(Msg, encoding = "utf-8"))
IRCMsg = IRCSocket.recv(4096).decode()
print(IRCMsg)

Msg = "PRIVMSG bot_b05703100 :!song 挪威的森林 \r\n"
IRCSocket.send(bytes(Msg, encoding = "utf-8"))
IRCMsg = IRCSocket.recv(4096).decode()
print(IRCMsg)

Msg = "PRIVMSG bot_b05703100 :Libra \r\n"
IRCSocket.send(bytes(Msg, encoding = "utf-8"))
IRCMsg = IRCSocket.recv(4096).decode()
print(IRCMsg)'''

