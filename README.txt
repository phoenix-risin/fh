################################# IRC BOT #################################

### Features
- Installierbar über makefile
- Antwortet auf private Nachrichten
- Konfigurierbar über Config-File
- IRC Channel Features (Topic ändern, Nick wechseln, Join/leave)

### Installieren

Aufrufen des Makefiles:
make

### Starten des Bots

Manuell:
Bot [servername] [channel] [nick]

Config:
Konfigurationsfile wird erstellt falls keines vorhanden ist.
Folgende Reihenfolge ist einzuhalten:

[servername] [channel] [nick]

### Kommandos

<Botname>	= Reagiert auf seinen Namen und antwortet
!nick Test	= Änderung des Nicknames in Test
!channel bla	= Wechseln des Channels zu "bla"
!topic blubb	= Ändern des aktuellen Channel-Topics zu "blubb"
!log		= Ein- oder Ausschalten des Loggings
!log_post	= Posten des Logs in den Channel
!quit		= Beenden