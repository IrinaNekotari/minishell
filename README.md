# Minishell 42
Le principe de minishell est de reconstruire un shell simplissime. L'execution se deroule en 3 etapes : la lecture de la commande, sa decomposition en un format exploitable, et l'execution a proprement parler.
> [!NOTE]
> Vous ne trouverez que du pseudocode dans le readme.
> Reflechissez par vous meme, ou tentez desesperement de dechiffrer notre code ...
> Mais n'oubliez pas - Ne copiez pas ce que vous comprenez pas !
## Lecture
La lecture se fait en grande partie via readline. Cette fonction permets de gerer facilement les buffers, l'historique, ainsi que les sauts de lignes via CTRL+C. Cependant, cette fonction peut egalemment generer de maniere relativement aleatoire des fuites memoires ...
La lecture est le "front" du minishell, a savoir ce qui interragit avec l'utilisateur. Il attends une commande, traite cette derniere, et recommence. Il n'a qu'une condition d'arret (ignorons le cas du exit pour l'instant), a savoir appuyer sur CTRL+D.
On a donc l'idee suivante :
```C
int  main(void)
{
  char  *buffer;
  while (1)
  {
      buffer = readline("minishell > ");
      if (ctrl_d)
        break ;
      execution(...);
      free(buffer);
  }
}
```
Contrairement a ce qu'on pourrait penser, CTRL+D n'est pas un signal : il est un caractere unique indiquant la fin de fichier, le EOF - End of File. Tapper CTRL + D dans un buffer vide enverra alors EOF dans notre execution. Cela risque peut etre de mener a des comportements indetermines ...
Cependant, ce n'est pas encore suffisant avant l'execution : il est d'abord necessaire de s'assurer que notre buffer est "ferme", a savoir
* Chaque `"` ouvrant doit avoir un `"` fermant, sauf s'il est entre deux `' '` ou precede d'un `\`
* Chaque `'` ouvrant doit avoir un `'` fermant, sauf s'il est entre deux `" "` ou precede d'un `|`
* Un `|` ne peut pas etre en fin ou debut de chaine (les espaces ne comptent pas)
* Pas plus de deux `||` peuvent se suivre
  
Le principe n'est pas complexe : on defini un int comme booleene (variable n'ayant que pour valeur possibles un FAUX (0) ou VRAI (1)), est un char de sauvegarde.

* Lorsque vous rencontrez un `"` ou un `'`, vous enregistrez ce dernier dans le char de sauvegarde, et mettez le booleen sur vrai.
* Lorsque vous rencontrez un charactere egal au caractere de sauvegarde, vous remettez la boolenne a 0.
* Si, a la fin de la chaine, la boolenne vaut vrai, ca veut dire qu'un `"` ou un `'` n'est pas ferme. Donc, renvoyez une erreur ...
> [!CAUTION]
> Un `"` ou un `'` precede d'un `\` ne compte pas !

> [!CAUTION]
> Double attention ! `\` "annule" `\`, donc `\\"` compte !

Pour le cas des pipes
* Si vous rencontrez un `|`, regardez les deux caracteres d'apres, en vous assurant de ne pas depasser la taille du buffer !
  * Si plus de 3 pipes se suivent, renvoyez une erreur.
* Si ce n'est pas le cas, regardez le caractere suivant.
  * Si c'est un espace, regardez celui d'apres.
  * Si vous arrivez a la fin de la chaine, renvoyez une erreur, car la chaine est finie par un `|`
  * Sinon, tout va bien - Vous pouvez continuer la verification.
> [!CAUTION]
> Tout comme au dessus, un `|` precede d'un `\` ne compte pas !

> [!CAUTION]
> Je ne mentionne que les espaces dans ce tutoriel, mais vous devez prendre en compte tous les whitespaces, comme les tabulations !

> [!TIP]
> Bien que le sujet ne le demande pas, au lieu de renvoyez une erreur, vous pouvez renvoyer le prompt, et ajouter le resultat a votre chaine, tout comme dans bash.
> Attention aux fuites memoires !

La verification est finie, mais c'est encore trop tot pour passer a l'etape suivante, car il reste un cas a traiter, celui du ;. On pourrait penser a faire simple, et faire un split sur votre buffer ...
Cependant, ce split aura un defaut fatal : il ne prends pas en compte les `"` et `'` ! Il faut alors recoder un split qui coupe la chaine en rencontrant un `;`, mais que si ce dernier n'est pas encadre par des `"` ou `'`. N'oubliez pas que ce genre de liste doit être terminée par un NULL.
> [!CAUTION]
> `\` est votre ennemi, ne l'oubliez pas !

> [!TIP]
> Pour vous aider pour la suite, durant vos vérifications, vous pouvez implémenter une fonction qui transforme, en dehors des zones fermées par des `"` ou des `'`, une suite d'espace de whitespace en un seul et unique espace, ce qui vous simplifiera la vie pour la suite.
On obtient donc le pseudocode suivant :
```C
int  main(void)
{
  char  *buffer;
  char  **tab;
  int  i;
  while (1)
  {
      i = 0;
      buffer = readline("minishell > ");
      if (ctrl_d)
        break ;
      if (!verif_quote || !verif_pipe)
        continue ;
      tab = split_semicolon(buffer);
      while (tab[i])
        execution(tab[i++]);
      free(buffer);
  }
}
```
## Decomposition (Parsing)
Avant de commencer la décomposition, ils faut vous demander - comment décomposer ?       
Lors de son éxecution, shell prends 3 élements en compte :
* La commande
* Ses arguments
* Possiblement une `|` qui s'ensuit sur une nouvelle commande

Pour simplifier les explications, chaque morceau a décomposer sera nommé token. Ainsi, si on prends l'exemple suivant

```echo -n bonjour les amis | cat ```

On aura la liste de tokens suivants :
* echo
* -n
* bonjour
* les
* amis
* |
* cat

Une première implémentation simpliste serait de split au niveaux des espaces. Mais, prenez en compte la commande suivante :

```echo bonjour "les                        amis"                 ça 'vaaaaa' ```

utiliser un split au niveau des espaces séparera "les" de "amis", qui ne sont pourtant qu'un seul et unique bloc. De plus,

```echo "bon"'jour'```

Un split ici ne séparera pas "bon" et "jour", qui sont pourtant deux blocs différents. Il va falloir faire autrement, créer un tokenizer, une fonction qui va découper notre buffer en une liste utilisable. Et, pour gérer une liste, quoi de mieux qu'une liste chainée ?

Considérons la liste chainée suivante : 
```C
typedef struct s_word
{
  char  *str;
  char  quote;
  int  has_space;
  struct s_word  *next;
  struct s_word  *previous;
}	t_word;
```
Cette liste chainée contient ce qu'on aura besoin pour notre tokenizer.
* str est le bloc en question.
* quote est un caractère de sauvegarde pour savoir si notre bloc est entouré de "/' ou non.
* has_space est un boolean, indiquant si le bloc est suivi d'un espace ou non, permettant de différencier les cas tels que ```"bon""jour"``` et ```"bon" "jour"```
* next et previous sont respectivement des pointeurs vers l'élément suivant et précédant de la liste, permettant un parcours efficace.

En soit, la réalisation du tokenizer n'est pas différent de celui de ft_split. Par contre, vous avez plus de données a prendre en compte :
* Un whitespace, en dehors d'une zone délimitée par des `'` ou `"`, signifie la fin du block courant et le début d'un nouveau
* lors de la création d'un bloc, vous devez ajouter a la liste par quel charactère le bloc est entouré (`"`, `'`, ou `0` sinon)
* Il existe des blocs spéciaux pour les redirections : `>`, `>>`, `<`, `<<`.
* Pour l'instant, nous allons ignorer l'existence de `|`.
> [!CAUTION]
> Si vous oubliez `\`, il viendra hanter vos nuits

Cette fonction est sans aucun doute la plus dure a écrire dans minishell. Nous obtenons le pseudocode suivant 
```C
void  add_token_to_list(t_word *liste, char *str, char quote, int has_space)
{
    if (!liste)
      initialisez la liste;
    else
    {
        allouez de la mémoire pour le pointeur suivant de la liste;
        réglez le pointeur sur le précedant du suivant a l'élément de liste en cours;
        déplacez l'élement en cours de la liste sur son suivant;
        initialisez l'élement suivant a NULL;
        dupliquez str dans liste->str;
        copiez quote et has_space dans leurs valeurs respective de la liste;
    }
}

char *get_word(char *str, int *i)
{
    char *word;
    char  quote;
    int j;

    Allouez de la mémoire a word;
    j = 0;
    if (str[*i] est un " ou un ')
    {
        quote = str[*i];
        (*i) += 1;
    }
    else
        quote = 0;
    while (str[*i])
    {
        if (quote == 0 && str[*i] est un whitespace OU un caractere special tel que >)
          break ;
        else if (str[*i] == quote)
        {
              (*i) += 1;
              break ;
        }
        word[j] = str[*i];
        j++;
        (*i) += 1;
    }
}

t_word  tokenizer(char *str)
{
  int  i;
  int boolean;
  char *add;
  t_word *liste;

  i = 0;
  boolean = 0;
  while (str[i])
  {
      while (str[i] est un whitespace)  
          i++;
      add = get_word(str, &i);
      add_token_to_list(...);
      i++;
  }
  
}
```
Malheureusement, trouver un compromis entre pseudocode et simplement copier nos fonction est compliqué ...     
Quoi qu'il en soit, vous avez maintenant un tokenizer basique. Il transforme la commande 

```echo -n "bonjour"> 'file.txt'```

En une liste chainée qu'on pourrait représenter ainsi

![Dessin sans titre(1)](https://github.com/Nolan-du76/minishell/assets/8365167/860874c3-9c3e-4134-aece-2e69c494a803)

Cependant, ce n'est pas encore suffisant. Et vous vous doutez de la première chose qu'on va devoir faire, puisque je vous ai demandé de l'ignorer temporairement - le cas du `|`.

Dans cette situation, nous allons crée une nouvelle liste chainée pour gérer les pipes. On en profite pour mettre quelques variables en plus qui nous serrons utiles plus tard ...
```C
typedef struct s_cmd
{
	struct s_io		*input;
	struct s_io		*output;
	struct s_word	*tokens;
	struct s_cmd	*pipe;
	struct s_cmd	*previous;
}	t_cmd;
```
Ignorons ce qu'est s_io pour l'instant, nous y viendrons juste après. Dans cette liste chainée, nous avons :
* un pointeur vers la liste de tokens
* un pointeur vers la commande suivante - celle située après le `|`
* un pointeur vers la commande précédente

L'initialisation de cette chaine est similaire a celle des tokens. Mais maintenant, comment la remplir ?

Vous vous souvenez sans aucun doute du split_semicolon plus haut; vous devez faire le même pour les `|`. Vous obtiendrez alors un tableau de `char *`, séparés par les `|` : vous n'aurez plus qu'a appeller votre tokenizer sur la ligne courante du tableau, déplacer l'élement courant de la liste sur son suivant, incrémentez la ligne courante du tableau, et recommencer jusqu'a la fin. 

Ce qui nous donne le pseudocode suivant : 
```C
t_cmd  *parse(char *line)
{
  char **splitz;
  t_cmd *cmd;
  int    i;

  i = 0;
  splitz = split_pipes(line);
  allouez de la mémoire a cmd;
  while (splitz[i])
  {
      appelez le tokenizer sur splitz[i];
      ajoutez son résultat dans cmd; inspirez vous de add_token_to_list;
      i++;
  }
  return (cmd);
}
```
Et voila - notre jolie commande est prête. Enfin, presque prête ... Qui sont ces `s_io` ? Et oui, encore une liste chainée, vous vous en doutez ...
```C
# define SINGLE_OUTPUT 1
# define DOUBLE_OUTPUT 2
# define SINGLE_INPUT 3
# define DOUBLE_INPUT 4

typedef struct s_io
{
	int			io;
	char		*file;
	struct s_io	*next;
	struct s_io	*previous;
}	t_io;
```
Vous commencez a avoir l'habitude, depuis le temps.
* io sert a stocker le type de redirection, définis par `SINGLE_OUTPUT`, `DOUBLE_OUTPUT`, `SINGLE_INPUT`, `DOUBLE_INPUT`, correspondant respectivement a `>`, `>>`, `<` et `<<`. Les valeurs sont purement arbitraires.
* file est le nom du fichier.
* Je passe next et previous, vous savez a quoi ils servent.

Comment les remplirs ? Nous allons parcourir notre belle liste chainée.
* Si le token est précisément égal a `>`, `>>`, `<`, `<<` ou `<>` (une exception bien bizzare, elle n'est pas demandé d'être gérée donc vous pouvez l'ignorer), vous retirez le token courant de la liste, et regardez le token suivant. En fonction de sa valeur, vous allez rendre un int de sauvegarde égal a `SINGLE_OUTPUT`, `DOUBLE_OUTPUT`, `SINGLE_INPUT`, ou `DOUBLE_INPUT`.
* S'il n'y pas de token suivant, ou que ce dernier est précisément égal a `>`, `>>`, `<`, `<<` ou `<>`, renvoyez une erreur, et quittez le traitement en cours.
* Sinon, il s'agit d'une redirection valable.
  * Si votre int de sauvegarde est `SINGLE_OUTPUT` ou `DOUBLE_OUTPUT`, vous ajouterez dans la liste chainée output de votre cmd.
  * Sinon, vous l'ajouterez dans la liste chainée input.
* Le token courant est le file que vous devrez plus tard ouvrir ou lire. Dupliquez le dans le char *file de votre liste chainée respective.
* Rendez égal io a votre int de sauvegarde.
* Supprimez le chainon courant
* Comme pour les listes chainées précedentes, passez au chainon suivant et recommencez.

> [!CAUTION]
> Attention ! `'>'` et `\\>` (entre autres) ne sont PAS des redirections !

Supprimer un chainon peut etre assez complexe, alors voici le pseudocode :
```C
/*
* Cette fonction assume que c pointe vers le chainon que vous voulez supprimer.
*/
void  delete_token(liste chainée c)
{
    on crée une copie de c;
    le pointeur previous du chainon suivant doit être mis égal au pointeur previous du chainon courant;
    le pointeur next du chainon précédant doit être mis égal au pointeur next du chainon courant;
    déplacez la chaine a son chainon suivant;
    faites un free sur toutes les valeurs a free de la copie;
    faites un free sur la copie;
}
```
La solution pour les deux lignes qui font peur :
```C
c->next->previous = c->previous;
c->previous->next = c->next;
```
Vous avez donc transformé la commande suivante

`echo bonjour les "amis" > file.txt | ls -la | cat`

en (pas de joli schéma cette fois)
```
cmd1 :
  tokens : echo->bonjour->les->"amis"
  output : file.txt (SINGLE_OUTPUT)
  previous : NULL;
  pipe : cmd2
cmd2 :
  tokens : ls->-la
  previous : cmd1
  pipe : cmd3
cmd3 :
  tokens : cat
  previous : cmd2
  pipe : NULL
```
Et voila le travail ! 

A moins que ...

![image](https://github.com/Nolan-du76/minishell/assets/8365167/f51927f6-33ce-4887-99c3-488a41531601)

Bien sûr - Vous avez encore du boulot !

L'une des fonctions de votre minishell demandée est de traiter les variables d'environement. Celles là même que vous pouvez tester dans shell :
`export $USER=test; echo Je suis $USER` et qui devra vous afficher `Je suis test`. Et pour ça, il va falloir revenir tout au début de votre code, dans le main ...
## Les variables d'environnement
Si vous avez déjà lancé `env` dans votre shell (ce que vous avez évidemment fait - env fait partie des builtins a coder, après tout), vous avez déjà vu les variables d'environnement : une tripotée de lignes de type `A=B`. Ces variables sont utilisées lorsque vous appelez des programmes par le shell. Elles permettent au programme d'avoir accès a votre environnement de travail, a savoir, entre autres
* `PWD=/home/user/folder/` : Le dossier courant du shell
* `PATH=/usr/local/sbin:/usr/local/bin` : Les chemins où sont enregistrés la plupart des programmes. Les chemins sont séparés par des :.
* `USER=c1r1p1` : L'utilisateur courant du shell.

Il y a bien plus de variables que ça, qui dépendent du shell que vous utilisez (bash ou zsh par exemple. valgrind et gdb ont eux aussi leurs propres environnements), mais ces 3 là sont les plus importantes pour minishell : Vous avez besoin de `PWD` pour pwd et cd, et `PATH` pour le cas général. 

Constatons autre chose. Si vous ouvrez deux shells différents, et que vous modifiez une variable d'environnement, vous remarquerez que modifier un ne modifiera pas l'autre. Chaque shell est indépendant, et son environnement aussi. Ce qui signifie une chose : minishell doit avoir son propre environnement. Et pour ça, commençons par récupérer l'environnement du shell courant.     

Pour créer votre fonction `main`, vous connaissez déjà deux façon de faire :
```C
int	main(void)
{
	return (0);
}
```
et
```C
int	main(int args, char *argv[])
{
	return (0);
}
```
Sachez qu'il en existe une troisième !
```C
int	main(int args, char *argv[], char *env[])
{
	return (0);
}
```
La variable `env` va récupérer automatiquement les variables d'environnement pour les stockers dans un tableau de string. Vous pouvez dores et déjà les afficher, et vous allez constater quelque chose : ce n'est pas très beau ou utilisable.
```C
int	main(int args, char *argv[], char *env[])
{
	int	i;

	i = 0;
	(void)args;
	(void)argv;
	while (env[i])
	{
		printf("Variable d'environnement numéro %d : %s\n", i, env[i]);
		i++;
	}
	return (0);
}
```
![image](https://github.com/Nolan-du76/minishell/assets/8365167/07ef842b-01bc-416e-9467-6a63e3bff729)

env copie "en dur" les variables d'environnement, a savoir au format `A=B`. Pas facile des les utiliser comme ça, sans compter que modifier un tableau, c'est galère : Il faut faire une copie, recalculer la taille, allouer de la mémoire pour chaque ligne ... La solution est donc très évidente :
### une liste chainée (encore une) !
```C
typedef struct s_env
{
	struct s_env		*next;
	struct s_env		*previous;
	char			*name;
	char			*value;
}	t_env;
```
Pas de surprises ici, il s'agit d'une chaine extrémement basique. Pour la créer, rien de plus simple :
* Vous découpez env[i] en deux morceaux : Tout ce qu'il y avant le =, tout ce qu'il y a après.
  * Attention ! Ne soyez pas tentez par le split (c'est le mal) ! `export A=B=C` est valide (bien que bizarre, on va pas se mentir), et la valeur de `A` ici vaut `B=C` !
* Ajoutez le nom (avant le =) et la valeur (après le =) a la liste.
* Passez au chainon suivant, incrémentez i, et recommencez jusqu'a atteindre la fin de env.

Ce qui nous donne en pseudocode :
```C
void	get_name_and_value(char *env, char **name, char **value)
{
	allouez de la mémoire a (*name) et (*value);
	parcourez env jusqu'a =;
		copiez env dans (*name);
	une fois le = rencontrez, continuez de parcourir env;
		copiez env dans (*value);
	Vous allez avoir besoin de deux int ...;	
}

t_env	*create_env(char **env)
{
	t_env *new_env;
	char *name;
	char *value;
	int i;

	i = 0;
	while (env[i])
	{
		get_name_and_value(env[i], &name, &value);
		add_to_env(new_env, name, value);
		free(name);
		free(value);
		i++;
	}
}
```
Vous êtes bien entendus libre de faire deux fonctions pour `get_name` et `get_value`. Cette solution est plus compact, pour appaiser la norminette ... 

Au passage, profitez en pour coder trois fonctions (dont une que vous avez de toute façon besoin pour initialiser l'environemment) : add_to_env, del_from_env, et get_env. Respectivement, ces fonctions vont vous permettre d'ajouter un élément a l'environemment (si l'élement existe déjà, sa valeur est mise a jour), de supprimer un élement de l'environnement, et de récupérer la valeur en fonction du nom passé. Ce qui nous donne les prototypes suivants :
```C
void	add_to_env(t_env *env, char *name, char *value);
void	del_from_env(t_env *env, char *name);
char	*get_env(t_env *env, char *name);
```
Et le pseudocode : 
```C
void	add_to_env(t_env *env, char *name, char *value)
{
	if (!env)
		initalisez env;
	else
	{
		parcourez la chaine;
			si vous trouvez une égalité entre env->name et name;
				supprimez env->value;
				dupliquez value dans env->value;
				quittez la fonction (c'est impossible d'avoir deux variables d'environnement avec le même nom);
		ajoutez le doublon name value a la fin de la chaine;
	}
}

void	del_from_env(t_env *env, char *name)
{
	if (!env);
		quittez la fonction;
	else
	{
		parcourez la chaine;
			si vous trouvez une égalité entre env->name et name;
				adaptez delete_token a env;
				quittez la fonction;
	}
}

char	*get_env(t_env *env, char *name)
{
	char *buffer;

	if (!env);
		quittez la fonction;
	else
	{
		parcourez la chaine;
			si vous trouvez une égalité entre env->name et name;
				dupliquez env->value dans buffer;
				renvoyez buffer;
	}
	return (NULL);
}
```
Et voila, votre environnement de travail est fin prêt ! On peut revenir au problème précédant, et, histoire de vous mettre la pression un coup, notre binôme c'est cassé les dents dessus !
## Convertir un $USER en sa valeur
Nous y voila - l'ultime étape de votre tokenizer. Créez une fonction get_variables, qui sera appelé AVANT la gestion des io (car oui, la série suivante `export $FILE=Truc.txt; echo bonjour > $FILE; cat Truc.txt` est parfaitement valide !), et qui va remplacer les variables de type `$USER` en leur valeur.

En voici le déroulement : 
* Parcourez votre chaine, chainon par chainon.
* Si le token a `'` pour quote, passez au suivant; `$` n'a pas a être interprété dedans.
* Sinon, parcourez la chaine de votre token.
* La variable que vous utilisez pour parcourir sera `i`, du moins pour le tutoriel.
* Si vous trouvez un `$` :
  * Assurez vous qu'il ne soit pas précédé d'un `\`. Si c'est le cas, ignorez le.
  * Assurez vous qu'il ne soit pas suivi d'un whitespace. Si c'est le cas, ignorez le.
  * S'il est suivi d'un `?` ... Ignorez ce cas pour l'instant, collez un bon gros `//TODO :` après le if, on y reviendra bien plus tard.
  * S'il est suivi d'un `$` ... Ce n'est pas un cas a traiter. Mais vous pouvez le faire si vous le souhaitez ! `$$` est remplacé par le PID du shell.
  * Sinon, considérez ceci :

![image](https://github.com/Nolan-du76/minishell/assets/8365167/66d0a226-3735-48f1-bf89-0c7d89dc6217)

  * Initialisez un int a `0`. Ce dernier nous servira a déterminer combien de caractères compose le nom de la variable.
  * Continuez de parcourir le token, en utilisant le nouvel int, que l'on va nommer `j`. Ne modifiez pas `i`, on en aura besoin ! Si vous rencontrez un whitespace, la fin de chaine, ou certains caractères (Je vous laissez les chercher, mais sachez que ces cas fourbes ne seront pas testés pendant la correction ...), il s'agit de la fin de notre variable.
  * Extrayez la sous-chaine et sauvegardez la : on en aura besoin pour récupérér la valeur de la variable.
  * Utilisez `get_env`, envoyez lui votre sauvegarde : vous avez maintenant récupéré la valeur de la variable.
  * ça va piquer : Vous allez devoir maintenant insérer la valeur dans votre chaine, en remplaçant le nom ET le `$`.
    * Calculez la taille de la nouvelle chaine, a savoir : `la taille de l'ancienne chaine + la taille de la valeur - (la longeur du nom de la variable + 1 (pour le $))`
    * Parcourez la chaine jusqu'a `i - 1`. Jusque la, recopiez bétement votre chaine dans la nouvelle.
    * Vous avez atteint `i - 1`, c'est a dire là ou se trouvait le `$` : Commencez a recopier la valeur dans votre nouvelle chaine.
    * Faites extrémement attention a ne pas recopier un `\0` en trop !
    * Vous avez atteint la fin de votre valeur. Vous pouvez continuez a copier votre chaine initiale ...
    * En prenant garde a ne pas recopier votre `$NOMDEVARIABLE`.
    * Vous avez atteint la fin de la chaine initiale. Féliciations, votre insertion est effectuée avec succès !
    * Vous n'avez plus qu'a remplacer la chaine du token par votre nouvelle chaine.
  * Mais pas la vérification : Recommencez sur la nouvelle chaine. Après tout, rien n'interdis l'utilisateur de votre minishell de mettre plusieurs variables dans le même token (le chieur) ...

C'était intense, c'était compliqué, je vous épargne le pseudocode, cette monstrueuse liste devrait suffire (je l'espère pour vous). Mais, regardez ce que vous avez accompli ! Le plus dur est derrière vous !
## Execution
Vous avez maintenant une belle liste paree a faire feu : il ne manque plus que l'execution. Si vous regardez ce que vous devez faire ici, vous pourriez etre tente de les separes en deux categories : les builtins, et le cas general. C'est faux : vous allez devoir les separer en trois categories. Mais, avant de les departager, regardons ce qu'on a faire :
> [!TIP]
> Avant de coder, ou meme de regarder ma jolie liste, faites les commandes vous meme dans un shell. Regardez ce qu'elles font, experimentez avec.

* echo
  * Affiche simplement un message.
  * Le flag `-n` doit etre gere, mais uniquement s'il vient apres le `echo`. Ce dernier supprime le retour a la ligne a la fin du message (Donc le prompt sera sur la meme ligne que le message).
* pwd
  * Affiche le repertoire courant.
  * Dans les variables d'environnement, vous aurez certainement apercus un `PWD` ...
* cd
  * Deplace le repertoire courant.
  * Vous allez devoir jouer avec le `PWD` des variables d'environnement.
  * Plusieurs situations a gerer :
    * Aucun argument : remets `PWD` a "Home", qui est au choix, le home classique du shell, ou le dossier ou est appele minishell.
    * Deux arguments ou plus : affiche un message d'erreur.
    * `..` : remonte l'arborescence d'un niveau (passe de `/a/b/c/d` a `/a/b/c`)
    * `.` : ne fait rien 
* export
  * Ajoute une nouvelle variable d'environnement. Si cette derniere existe deja, la mets a jour.
  * Il est possible d'exporter plusieurs variables d'environnement en une commande.
  * Verifiez ce qui renvoie des erreurs : par exemple, `export 0j=Test` ...
  * Verifiez ce qui ne renvoie PAS d'erreur, mais ne fait rien pour autant : par exemple, `export test` ...
  * Melangez. Que ce passe t'il quand on mets plusieurs variables, mais qu'une doit renvoyer une erreur ?
  * Le sujet ne demande pas de traiter ce cas, mais que se passe t'il si vous envoyez juste `export` ?
* unset
   * Retire une variables d'environnement.
   * Comme pour export, vous pouvez unset plusieurs variables a la fois.
   * Effectuez les memes verifications, et regardez ce qu'il se passe.
   * unset les variables d'environnement vitales (`PATH` ou `PWD` par exemple) est possible. Vous devrez sans aucun doute prevoir quelque chose dans votre code pour ca ...
* env
  * Affiche la liste des variables d'environnement, ni plus ni moins.
  * Le sujet ne vous demande pas de traiter les arguments. Mais que se passe t'il si vous envoyez `env a` ? Ou bien `env a=b` ? Reesayez env seul apres.
* exit
  * Quite le minishell de maniere propre.
  * Par defaut, quitte le minishell avec un code de retour egal a `0`.
  * Si vous mettez un argument apres le exit, le code de retour sera egal au dit argument. Utilisez atoi ...
  * Si vous en mettez plus, un message d'erreur sera affiche (et le minishell, pas quitte) 
* Cas general
  * Sinon, vous envoyez la commande au systeme, et vous le laissez se depatouiller avec. Simple, non ?

Vous pouvez voir que trois categories se demarquent : Les commandes qui affichent quelque chose, les commandes qui modifient l'environnement (exit est inclus dedans), et le reste. Maintenant, voyons une petite particularite des commandes qui modifient l'env (que nous allons appeller commande systeme).
* Testez `export A=B; env; unset A; env`. Regardez ce que ca affiche.
  * L'environnement est affiche deux fois. La premiere fois, A est bien ajoute, et la deuxieme fois, il est bien supprime.
* Testez `export A=B | env`.
  * A n'est pas ajoute.
* Testez `env`, juste histoire d'etre sur.
* Testez `echo test | cd ..`. Vous vous deplacez bien dans l'arborescence.
* Testez `exit | echo nope`.
* Testez `ls -la | cd .. | cat`.
  * Non seulement `cd ..` ne fait rien, mais `cat` n'arrive pas a recuperer ce que `ls -la` affiche.

Ces commandes systemes ont donc deux particularites : Non seulement elle ne font quelque chose QUE quand elles sont les dernieres commandes effectuees, elle "bloquent" les pipes si utilises en pleine commande !

Votre execution va d'abord devoir faire deux branches : si la commande est un systeme, ou non. N'oublions pas : la commande est le premier token.
```C
if (premier token est un systeme)
	execute_system(...);
else
	execute_others(...);
```
`exit`, `unset` et `export` sont triviaux a faire : vous avez deja fait 95% du boulot avec `del_from_env` et `add_to_env`. Vous n'avez plus qu'a ajouter les verifications. Quant a exit, n'oubliez pas de faire les free ... 

`cd` peut s'averer un petit peu plus complique. Vous pouvez etre tente d'utiliser `chdir` ... Mais ce dernire a un probleme. `chdir` va bien modifier le repertoire courant ... Du shell, pas du minishell ! Vous pouvez tenter de le recoder, et bonne chance dans ce cas, ou sinon, vous pouvez l'utiliser de maniere detournee :
* Apres avoir fait vos verifications, envoyez l'argument a `chdir`.
* `chdir` va renvoyer un int. Si ce dernier vaut -1, `chdir` n'as pas reussi a changer de dossier, pour une raison quelconque.
  * Affichez une erreur !
* Sinon, `chdir` modifie le dossier courant du shell, mais pas du minishell. Mince alors !
* Pas de panique ! Vous avez le droit d'utiliser `getcwd`, qui renvoie le dossier courant ... Du shell, pas du minishell.
* Vous voyez ou je veux en venir : recuperez le dossier courant du shell, puis utilisez le pour mettre a jour l'env du minishell via `add_to_env`.
* Une petite subtilite : Remettez le dossier courant du shell (pas du minishell !) a sa valeur initiale, a savoir dans le dossier ou vous avez appelle minishell. Ne vous inquietez pas, on y reviendra plus tard ...
* Il vous reste le cas de `cd` a traiter. Ce cas necessite une sauvegarde du `PWD` initial - c'est aussi ce que vous aurez besoin au dessus. Nous y reviendrons plus tard.

Les cas systemes sont codes. Jettons un oeil aux fonctions d'affichage ...
> [!TIP]
> Si vous avez decide d'implementer `export` sans arguments, ce dernier compte aussi comme un affichage !

`echo`, `pwd` et `env` sont relativement triviaux. Sachez cependant que pour la suite, ces deux fonctions ne doivent afficher qu'un seul et unique `char *`, et non pas une boucle de print !   

Leur vraie difficulte viendra des redirections ...
### Gerer les redirections
Gerer les redirection se fera en deux temps : d'abord les redirections de type `<` et `<<`, AVANT la commande, et les redirections de type `>` et `>>` apres la commande.
* Testez ce que fait `<`.
  * Le shell n'effectue la commande que si la redirection existe. Si elle n'existe pas, elle affichera une erreur.
  * Testez une commande avec plusieurs `<`. La commande ne sera executee que si TOUTES les redirections existe.
  * Qu'en est-il s'il y a une pipe dans la commande ?
* Testez ce que fait `<<`.
  * (Absolument infect)
  * `<<` renvoie un prompt, qui tourne en boucle jusqu'a ce qu'il soit interrompu (CTRL+C), qu'il lise un EOF (CTRL+D), ou que le buffer soit precisment egal a la redirection.
  * Donc, `echo test << a` tournera en boucle jusqu'a ce vous entriez `a`.
  * Testez avec plusieurs `<<`. Ca tourne au ridicule.
  * Cette situations renverra un prompt, jusqu'a ce que vous entriez toutes les redirections dans un ordre precis :
    * La derniere, l'avant derniere ... La seconde, la premiere, la seconde ... L'avant derniere, la derniere.
  * Ou ont-ils ete cherche ca, serieux ?
* Testez ce que fait `>`.
  * Au lieu d'afficher quelque chose, la commande va imprimer dans un fichier.
  * Si le fichier n'existait pas, il le cree avant d'imprimer.
  * Sinon, il vide ce qu'il avait dedans avant d'imprimer.
  * Testez plusieurs `>`.
  * Testez une pipe apres, par exemple `echo test > a | cat`. 
* Testez ce que fait `>>`.
  * Au lieu d'afficher quelque chose, la commande va imprimer dans un fichier.
  * Si le fichier n'existait pas, il le cree avant d'imprimer.
  * Sinon, elle imprimera apres ce qui existait deja.
  * Testez plusieurs `>>`.
* Melangez les toutes, amusez vous.

Le deroulement de l'execution devra se derouler ainsi :
* Testez les redirections `<` et `<<`. Si elles ne sont pas valides, quittez la commande.
* Effectuez la commande.
* Si elle n'as pas de redirections `>` ou `>>`, affichez le resultat.
* Si elle a des redirections `>` ou `>>`, imprimez dans les fichier.
* Si elle a des redirections `>` ou `>>` ET une pipe, imprimez ET affichez.
* Passez a la pipe suivante si elle existe.
* Recommencez.

Vous pouvez d'ores et deja tester votre code, et il marche ! Mais vous allez vite remarquez que ca coince a un stade. Regardez pour `echo a | echo b`, et comparez le a celui du shell ... 
### L'etrange cas des forks et des pipes
On rentre enfin dans le vif du sujet, le coeur meme de minishell, la raison pour laquelle on fait tout ca : les forks, et les pipes. 

Votre code d'execution ressemble a quelque chose comme ca pour l'instant : 
```C
void	execution(t_cmd *cmd)
{
	if (premier token est un systeme)
		execute_system(...);
	else
		execute_others(...);
	if (cmd->pipe)
		execute(cmd->pipe);
}
```
Votre `execute_others` etant une simple liste de conditions qui teste les builtins. Il va falloir changer ca. Mais avant, quelques definitions :
> La fonction fork fait partie des appels système standard d'UNIX (norme POSIX1). Cette fonction permet à un processus (un programme en cours d'exécution) de donner naissance à un nouveau processus qui est sa copie conforme, par exemple en vue de réaliser un second traitement parallèlement au premier. Un bon moyen de visualiser l'effet d'un fork sur un processus est d'imaginer une division cellulaire. 

(Wikipedia)[https://fr.wikipedia.org/wiki/Fork_(programmation)]

>  En génie logiciel, un tube ou une pipeline est un mécanisme de communication inter-processus sous la forme d'une série de données, octets ou bits, accessibles en FIFO. Le patron de conception qui correspond à ce mécanisme s'appelle le filtre. Les tubes des shell, inventés pour UNIX, permettent de lier la sortie d'un programme à l'entrée du suivant dans les shell et permet, tel quel, de créer des filtres.

(Wikipedia)[https://fr.wikipedia.org/wiki/Tube_(informatique)]

Pour que votre programme communique un resultat a la commande suivante, vous allez devoir utiliser les tubes (pipe) en anglais. Vous pouvez vous representer un pipe comme un tube (d'ou le nom) avec une entree et une sortie. Ce que vous ecrirez dans l'entree sera lue dans la sortie; et, en ecrivant dans l'entree d'un pipe ce qu'il y a dans la sortie d'un autre, vous creerais une pipeline. C'est exactement ce qu'on a besoin pour notre minishell : imaginons la commande suivante :

`ls -la | grep dr | cat`

Si vous utilisez juste `ls -la`, vous verrez quelque chose de ce genre :
```
total 4
drwxr-xr-x  5 nmascrie 2023_le-havre   49 Dec 19 07:55 .
drwx------ 22 nmascrie 2023_le-havre 4096 Dec 19 09:55 ..
drwxr-xr-x  6 nmascrie 2023_le-havre  160 Dec 19 08:05 githubshell
drwxr-xr-x  6 nmascrie 2023_le-havre  149 Dec 12 09:41 mini
drwxr-xr-x  6 nmascrie 2023_le-havre  146 Dec 19 07:55 test
```
Ici, il n'y a pas de |, donc pas de pipe a ouvrir. `ls -la` va lire sur l'entree standard (`STDIN` ou `0`) - dans cette situation, ls ne lis rien car il n'a pas besoin de lire quoi que ce soit - et ecrire dans la sortie standard (`STDOUT` ou `1`)
. 

Prenons maintenant `cat`. Si vous lancez cat seul, il va boucler en attendant une entree : ce que vous ecrivez sera affiche immediatement a l'ecran, et `cat` attendra a nouveau une commande, jusqu'a ce que vous le quittiez via CTRL+C ou CTRL+D. Vous l'avez compris : `cat` lit sur l'entree standard, et affiche immediatement sur la sortie standard. Maintenant, que se passe t'il si on utilise `cat test.txt` ? Au lieu de lire sur l'entree standard, `cat` va ouvrir un flux vers `test.txt` (via un File Descriptor, fd), et afficher son contenu sur la sortie standard. Dans un sens, c'est plus ou moins un pipeline : mais au lieu d'ouvrir un pipe, on ouvre un fichier. 

Passons a notre cas initial, a savoir `ls -la | grep dr | cat`.
* `ls -la` va afficher ce qu'il doit afficher sur la sortie standard. Cependant, vu qu'il y a un | apres, il va ecrire dans l'entree de la pipe plutot que sur la sortie standard.
* `grep dr` va lire sur l'entree standard. Mais, comme il y a un | avant, il va lire dans la sortie de la pipe. Et, comme il y a un autre | apres, il va ecrire a nouveau dans la pipe.
* `cat` vous l'avez devine : plutot que de lire dans l'entree standard, il va lire la sortie de la pipe. Et, vu qu'il n'y a plus de |, il va afficher tout ca dans la sortie standard.

Comment retranscrire tout ca en C ? commencons par le code suivant : 
```C
int	fd[2];
int	i;

i = pipe(fd);
if (i == -1)
	return ;
```
Ici, nous allons creer un pipe a l'aide de `fd`. Comme dit plus haut, `fd` a besoin de deux valeur : son entree et sa sortie. `pipe()` va parametrer ces deux valeurs : ainsi, `fd[0]` sera la sortie de notre pipe et `fd[1]` son entree (oui, c'est inverse par rapport au Standard). On recupere le code de sortie de `pipe` dans `i`; si ce dernier vaut `-1`, c'est qu'il y a eu un caffouillage au niveau du systeme (donc, pas de votre faute). Dans cette situations, affichez un joli message d'erreur, et quittez la commande. Sinon, on peut passer a la suite : le fork. Considerons le code suivant :
```C
int	fd[2];
int	i;
int	pid;
int	retour;

i = pipe(fd);
if (i == -1)
	return ;
pid = fork();
if (pid == -1)
	return ;
if (pid == 0)
	do_stuff(...);
waitpid(pid, &retour, 0);
do_things(...);
```
Une fois notre pipe ouverte, on va forker. Si la definition de Wikipedia n'etait pas claire, un fork est une copie du programme, avec exactement les memes valeurs, et qui commence au moment de l'appel du fork. La seule difference ? Le PID du fork est 0. Donc, nous avons :
* La creation du fork, et le stockage de son PID dans `pid`.
* Si `pid` vaut -1, le systeme a plante quelque part, donc, vous devez quitter. N'oubliez pas un delicieux message d'erreur.
* Si `pid` vaut 0, vous etes dans le fork, communement appele fils. Donc, vous faites ce que le fork a a faire.
* `waitpid(pid, &retour, 0)` va attendre la fin de l'execution du fork, et stocker son code de retour dans `retour`. Ici, 0 definit les options du `waitpid`. On en a pas besoin, mais si vous etes curieux, n'hesitez pas a aller regarder quelles sont ces options dans le man. Le programme non-fork (appele pere) va donc attendre la fin du fils avant de continuer.
* Une fois le fils termine, on effectue la fonction `do_things()`.

C'est ici que ca se complique. Considerons que votre fonction `do_stuff()` est la suivante :
```C
void	do_stuff(t_cmd *cmd, int *fd)
{
	if (cmd->tokens->str est un builtin)
		do_builtin(...);
	else
		do_general(...);
	exit (0);
}
```
Notez bien qu'on `exit` le fork ici.

Si vous executez, vous verez que le probleme est toujours le meme : `echo a | echo b` affiche toujours `a \n b`. En effet : le pipe est ouvert, mais vous n'ecrivez pas dedans. Il va falloir dupliquer : c'est a dire, remplacer la sortie standard par celle de notre pipe.
```C
dup2(fd[1], 1);
```
Ici, on va dupliquer l'entree de notre pipe dans la sortie standard. Ne testez pas encore (ou allez-y, je ne suis pas votre pere), il reste des choses a faire.
* Si la commande n'a pas de pipe, on a pas besoin de dupliquer, vu qu'on utilise la sortie standard.
* Il faut fermer les flux apres les avoir utilise.
* La fermeture et la duplication peut echouer ! Il faut s'en premunir.

Ce qui nous donne une bonne grosse condition :
```C
else if (cmd->pipe && (dup2(fd[1], 1) == -1
			|| close(fd[1]) == -1
			|| close(fd[0]) == -1))
	exit (1) ;
```
Ici tout est fait dans une seule condition, mais libre a vous de la separer. Mais, ce n'est pas encore suffisant ! Vous avez duplique l'entree de votre pipe ... Mais pas sa sortie ! Allons donc dans la fonction `do_things()`.
```C
void	do_things(t_cmd *cmd, int *fd)
{
	???;
}
```
On va dupliquer ici aussi :
```C
void	do_things(t_cmd *cmd, int *fd)
{
	else if (cmd->pipe && (dup2(fd[0], 0) == -1
			|| close(fd[1]) == -1
			|| close(fd[0]) == -1))
	exit (1) ;
}
```
Vous remarquez ici qu'on duplique la sortie de notre pipe `fd[0]` dans l'entree standard `0`. C'est un debut : il va maintenant falloir faire en sorte que le fork s'effectue sur toutes les commandes de notre liste chainee ... Et, au passage, s'assurer de bien executer les systemes au besoin. 
* Si la commande actuelle est un systeme ET n'as pas de pipe, excutez le systeme.
* Sinon, ouvrez le pipe et lancez le fork.
  *  Dans le fork, dupliquez l'entree de la pipe dans la sortie standard.
  *  Si la commande est un systeme ET qu'il y a une pipe, la commande va "bloquer" le pipe, le vider (souvent dit "flush"). Dans ce cas, rien de plus complique : Vous n'avez qu'a print une chaine vide `printf("");`.
  *  Sinon, si la commande est un builtin, executez la.
  *  Sinon, executez le cas general.
  *  Quittez le fork avec un `exit`.
    * Vous aurez besoin de recuperer le code de sortie a un moment ou un autre. Donc, vous pourrez par exemple faire en sorte que vos builtins renvoient un 1 s'ils echouent pour une quelconque raison ...
  * Si la commande possede une pipe, recommencez de maniere recursive.
 
> [!CAUTION]
> Vos commandes systemes doivent elle aussi renvoyer un code d'erreur !

Il s'agit du deroulement general de l'execution. Vous aurez certains cas supplementaires a gerer : par exemple, si vous avez des redirections, vous duplications seront sans doutes legerement differentes ... Mais si vous avez survecu et compris jusqu'ici, vous devriez sans doute pouvoir le faire seuls ! Et sinon, c'est dans `src/execute/bin_forks.c` !

Il nous reste un dernier morceau a degrossir : le cas general. Et pour cette situation, on a notre seigneur et sauveur `execve`. Regardez un oeil dans le man avant de continuer.
* `execve` prends trois arguments : un executable, ses arguments (les arguments DOIVENT contenir l'executable en lui meme !) et l'environnement.
* l'executable est un chemin vers un fichier qui peut etre execute : par exemple, quand vous lancez `ls` dans le shell, vous utilisez l'executable `/usr/bin/ls`
* Les arguments sont un tableau de char *. Dans le cas de `ls -l -a`, le tableau sera compose de `ls`, `-l` et `-a`
* L'environnement est un tableau de char *. Vous vous souvenez, celui dans le main ?

La premiere etape va etre de creer nos deux tableaux. A ce stade, vous n'avez certainement pas besoin d'indication pour ca, non ? Si oui ... Etudiez le pseudocode suivant :
```C
char **list_to_array(liste *l)
{
	int taille;
	char **ret ;
	int	i;

	taille = 0;
	while (l)
	{
		ajouter a taille la taille du char * de l'element courant;
		pensez a ajouter 1 a la taille pour le = dans le cas de t_env;
		Passez au chainon suivant;
	}
	Rembobinez votre chaine au debut;
	ajouter a taille assez de place pour un NULL;
	i = 0;
        while (l)
	{
		creez une chaine issue de la concatenation entre l->name, "=" et l->value pour t_env;
		Dupliquez cette chaine dans ret[i]. Dans le cas de t_word, vous pouvez directement dupliquer sa chaine.
		i++;
		passez a l'element suivant; 
	}
	ret doit etre termine par un NULL;
	return (ret);
}
```
Adaptez ce code pour vos deux listes chainees. Passons au cas de l'executable. Vous avez sans doute remarque dans mon exemple que le chemin `/usr/bin/` apparait dans `PATH` de votre environement. En effet, `PATH` contient tous les chemins vers les dossiers des executables. Donc, pour chaque commande, vous allez devoir tester si votre executable est inclus dans les `PATH`;
* Vous pouvez voir que les `PATH` sont separes par des `:`. Vous pouvez donc utiliser un split dessus sans danger.
* Pour chaque `PATH` :
  * Creez une chaine issue de la concatenation du `PATH` et de votre commande.
  * Envoyez cette chaine a `execve`, avec les deux tableaux crees au dessus, et enregistrez le retour de `execve` dans un int.
  * Liberez la memoire de la chaine, et passez au `PATH` suivant.
* Pour finir, tentez d'executer `execve` en local, c'est a dire sans ajouter de `PATH`, par exemple pour lancer minishell dans votre minishell.
* Liberez la memoire ...
* Retournez l'int.

> [!CAUTION]
> On va vous demander d'evaluer les `PATH` de droite a gauche plutot que de gauche a droite ! Cela signifie que le dernier `PATH` doit etre essaye en premier !

Vous allez me faire remarquer quelque chose ... Si le premier `execve` marche, pourquoi continuer la boucle ? Pourquoi essayer les autres `PATH` ! Et bien, figurez vous que tout est calcule ! `execve` est une fonction magique qui QUITTE le programme s'il est reussi ! C'est pourquoi on a besoin de le faire dans un fork : sinon, il quitterait notre minishell ... En plus, il s'occupe meme des fuites memoires.     

Vous pouvez lancer votre minishell - il est presque fini ! Tout marche et ... Oh. Faire une commande avec un | ferme le minishell. `$?` et `cd` ne sont toujours pas gere. 

Pour y remedier, on va avoir besoin de ...

Vous l'avez devine ... 

Une ...

### STRUCTURE !

Et oui, pas de liste chainee cette fois.
## La structure main, fourre-tout universel
Pour resoudre ces trois soucis, on va creer une structure nomme main, qui fera office de fourre-tout (vous en aurez besoin pour free). Cette structure contiendra entre autre un pointeur vers votre environnement, une copie du `PWD` au lancement du minishell, un int qui stockera les codes de retour, et un fd de secours.
```C
typedef struct s_main
{
	int	code;
	int	backup[2]
	char	*init_pwd;
	t_env	*env;
}	t_main;
```
* Au lancement de votre minishell, utilisez `getcwd` pour creer `init_pwd`.
* Au meme moment, initialisez `env` avec la fonction que vous avec cree plus tot.
* Encore au meme moment, vous allez dupliquer l'entree et la sortie standard dans vos backup. utilisez `backup[0] = dup(0);` et `backup[1] = dup(1);`
* Dans votre execution, apres le `waitpid`, rendez egal votre `code` au `retour`.

Cette structure permets maintenant de regler tous les soucis :
* Dans votre boucle principale, avant d'appeler `readline`, retablissez les entrees et sorties standards a l'aide de vos backups et `dup2`.
* Dans votre `get_variable`, vous pouvez maintenant gerer le cas du `$?`. Vous n'avez plus qu'a utiliser `ft_itoa` sur `code`, et l'inserer dans votre token.
* Vous pouvez maintenant terminer `cd` avec `initpwd` : utilisez le pour retablir `chdir`, et pour le cas du `cd` seul.

Votre minishell est terminee ! Vous n'avez plus qu'a chasser les fuites memoires (Bonne chance) et norminer tout ca (Bonne chance<sup>2</sup>).
## Details
Quelques petites astuces !
* Utilisez `ft_calloc` plutot que `malloc`.
* Si vous n'avez pas fait Minitalk, jetez-y un oeil - il vous explique comment gerer les signaux.
* Le manuel de `readline` a tout ce qu'il faut pour gerer l'historique et le CTRL+C.
* Ajoutez des couleurs ! Non seulement c'est joli, mais en plus ca vous aidera a vous reperez dans votre minishell.
* `\` est l'ennemi, ne l'oubliez JAMAIS
* Si jamais vos fonctions ne modifient pas vos listes chainees, et que vous etes certains que le probleme ne vienne pas d'autre part
  * Modifiez les fonctions de modification de chaine : au lieu de prendre le pointeur d'une liste, elle prendront le pointeur du pointeur.
  * `void list_func(t_liste *li);` devient `void list_func(t_liste **li);`
  * Dans le corps de la fonction, n'oubliez pas de les dereferencer !
  * `l->next` devient alors `(*l)->next`
  * Et quand vous appelez ces fonctions, envoyez leur le pointeur de la liste !
  * `modify_list(liste)` devient alors `modify_list(&liste)`
* Si vous modifiez vos fonctions ainsi, vous allez rencontrer un autre soucis. Apres les fonctions de modification ou de parcours de la liste, la liste restera a sa derniere position, ce qui est en general la fin de la liste.
  * Vous allez devoir "rembobiner" cette liste. C'est a dire, revenir au premier element.
  * Deux moyens de faire ainsi : le premier est de stocker quelque part le pointeur du premier element, par exemple dans la structure `main`.
  * La deuxieme facon est d'utiliser les pointeurs `previous`. Tant que ce pointeur n'est pas nul, vous deplacez l'element a son precedent.
* Telle une cascade, vous allez rencontrer un 3em soucis.
  * Si vous vous deplacez sur un `NULL` (fin de chaine), et que vous tentez de rembobiner apres, vous allez planter.
  * Apres tout, `NULL` n'as pas de precedent.
  * Il faudra ajouter une condition dans votre boucle de parcours : si l'element n'as pas de suivant, alors on quitte la boucle.  
