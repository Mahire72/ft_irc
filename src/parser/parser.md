# Question: Qu'est-ce que extractCommand() me donne exactement ?
"NICK bob"  ===> notre parser doit simplement trouver le premier espace.
`command` = "NICK"
'args` = "bob"

`----------------------`
## rest: PASS, NICK, USER, REGISTRATION, STATE, 001 Welcome, CAP

### 1. Registration state: a faire en premier.
User have `bool _isRegistered` mais on doit savoir separement:

                PASS → reçu ?
                NICK → reçu ?
                USER → reçu ?
PAS, NICK, USER  pas encore registered. puis pass, nick, user are registered.

#### implementer PASS
Quand le parser donne:
                commandName = "PASS"
                arguments = "secret"
on  doit verifier le mot de passe du serveur.
                    PASS secret
                        ↓
                    compare with server password
                        ↓
                    correct ?
                    ┌──┴──┐
                    YES    NO
                    ↓      ↓
                _hasPass  error
                  = true
parser seulement repondre quelle est la commande et quelle est ses arguments. donc ile ne devrait pas connaitre la configuration de ton serveur. `le mot de passe est une propriete du serveur`.

`la logique`
                Client
                ↓
                "PASS secret123"
                ↓
                Parser
                ↓
                command = "PASS"
                arguments = "secret123"
                ↓
                Server vérifie :
                "secret123" == mon mot de passe ?
                            ↓
                    ┌───────┴───────┐
                    oui             non
                    ↓               ↓
                _hasPass = true     erreur

puisque our serveur sera lance par: `./ircserv 6667 secret123` 
donc main:
int main(int argc, char **argv)
{
    Server server(atoi(argv[1]), argv[2]);
    Server.start();
}