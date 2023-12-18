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
* Chaque " ouvrant doit avoir un " fermant, sauf s'il est entre deux ' ' ou precede d'un \
* Chaque ' ouvrant doit avoir un ' fermant, sauf s'il est entre deux " " ou precede d'un |
* Un | ne peut pas etre en fin ou debut de chaine (les espaces ne comptent pas)
* Pas plus de deux || peuvent se suivre
  
Le principe n'est pas complexe : on defini un int comme booleene (variable n'ayant que pour valeur possibles un FAUX (0) ou VRAI (1)), est un char de sauvegarde.

* Lorsque vous rencontrez un " ou un ', vous enregistrez ce dernier dans le char de sauvegarde, et mettez le booleen sur vrai.
* Lorsque vous rencontrez un charactere egal au caractere de sauvegarde, vous remettez la boolenne a 0.
* Si, a la fin de la chaine, la boolenne vaut vrai, ca veut dire qu'un " ou un ' n'est pas ferme. Donc, renvoyez une erreur ...
> [!CAUTION]
> Un " ou un ' precede d'un \ ne compte pas !

> [!CAUTION]
> Double attention ! \ "annule" \\, donc \\\\" compte !

Pour le cas des pipes
* Si vous rencontrez un |, regardez les deux caracteres d'apres, en vous assurant de ne pas depasser la taille du buffer !
  * Si plus de 3 pipes se suivent, renvoyez une erreur.
* Si ce n'est pas le cas, regardez le caractere suivant.
  * Si c'est un espace, regardez celui d'apres.
  * Si vous arrivez a la fin de la chaine, renvoyez une erreur, car la chaine est finie par une |
  * Sinon, tout va bien - Vous pouvez continuer la verification.
> [!CAUTION]
> Tout comme au dessus, un | precede d'un \ ne compte pas !

> [!CAUTION]
> Je ne mentionne que les espaces dans ce tutoriel, mais vous devez prendre en compte tous les whitespaces, comme les tabulations !

> [!TIP]
> Bien que le sujet ne le demande pas, au lieu de renvoyez une erreur, vous pouvez renvoyer le prompt, et ajouter le resultat a votre chaine, tout comme dans bash.
> Attention aux fuites memoires !

La verification est finie, mais c'est encore trop tot pour passer a l'etape suivante, car il reste un cas a traiter, celui du ;. On pourrait penser a faire simple, et faire un split sur votre buffer ...
Cependant, ce split aura un defaut fatal : il ne prends pas en compte les " et ' !. Il faut alors recoder un split qui coupe la chaine en rencontrant un ;, mais que si ce dernier n'est pas encadre par des " ou '. N'oubliez pas que ce genre de liste doit être terminée par un NULL.
> [!CAUTION]
> \ est votre ennemi, ne l'oubliez pas !

> [!TIP]
> Pour vous aider pour la suite, durant vos vérifications, vous pouvez implémenter une fonction qui transforme, en dehors des zones fermées par des " ou des ', une suite d'espace de whitespace en un seul et unique espace, ce qui vous simplifiera la vie pour la suite.
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
* Possiblement une | qui s'ensuit sur une nouvelle commande

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
* Un whitespace, en dehors d'une zone délimitée par des "/', signifie la fin du block courant et le début d'un nouveau
* lors de la création d'un bloc, vous devez ajouter a la liste par quel charactère le bloc est entouré (", ', ou 0 sinon)
* Il existe des blocs spéciaux pour les redirections : >, >>, <, <<.
* Pour l'instant, nous allons ignorer l'existence de |.
> [!CAUTION]
> Si vous oubliez \, il viendra hanter vos nuits

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

Cependant, ce n'est pas encore suffisant. Et vous vous doutez de la première chose qu'on va devoir faire, puisque je vous ai demandé de l'ignorer temporairement - le cas du |.

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
* un pointeur vers la commande suivante - celle située après le |
* un pointeur vers la commande précédente

L'initialisation de cette chaine est similaire a celle des tokens. Mais maintenant, comment la remplir ?

Vous vous souvenez sans aucun doute du split_semicolon plus haut; vous devez faire le même pour les |. Vous obtiendrez alors un tableau de char *, séparés par les | : vous n'aurez plus qu'a appeller votre tokenizer sur la ligne courante du tableau, déplacer l'élement courant de la liste sur son suivant, incrémentez la ligne courante du tableau, et recommencer jusqu'a la fin. 

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
Et voila - notre jolie commande est prête. Enfin, presque prête ... Qui sont ces "s_io" ? Et oui, encore une liste chainée, vous vous en doutez ...
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
* io sert a stocker le type de redirection, définis par `SINGLE_OUTPUT`, `DOUBLE_OUTPUT`, `SINGLE_INPUT`, `DOUBLE_INPUT`, correspondant respectivement a >, >>, < et <<. Les valeurs sont purement arbitraires.
* file est le nom du fichier.
* Je passe next et previous, vous savez a quoi ils servent.

Comment les remplirs ? Nous allons parcourir notre belle liste chainée.
* Si le token est précisément égal a >, >>, <, << ou <> (une exception bien bizzare, elle n'est pas demandé d'être gérée donc vous pouvez l'ignorer), vous retirez le token courant de la liste, et regardez le token suivant. En fonction de sa valeur, vous allez rendre un int de sauvegarde égal a `SINGLE_OUTPUT`, `DOUBLE_OUTPUT`, `SINGLE_INPUT`, ou `DOUBLE_INPUT`.
* S'il n'y pas de token suivant, ou que ce dernier est précisément égal a >, >>, <, << ou <>, renvoyez une erreur, et quittez le traitement en cours.
* Sinon, il s'agit d'une redirection valable.
  * Si votre int de sauvegarde est `SINGLE_OUTPUT` ou `DOUBLE_OUTPUT`, vous ajouterez dans la liste chainée output de votre cmd.
  * Sinon, vous l'ajouterez dans la liste chainée input.
* Le token courant est le file que vous devrez plus tard ouvrir ou lire. Dupliquez le dans le char *file de votre liste chainée respective.
* Rendez égal io a votre int de sauvegarde.
* Supprimez le chainon courant
* Comme pour les listes chainées précedentes, passez au chainon suivant et recommencez.

> [!CAUTION]
> Attention ! '>' et \\> (entre autres) ne sont PAS des redirections !

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
* Si le token a ' pour quote, passez au suivant; $ n'a pas a être interprété dedans.
* Sinon, parcourez la chaine de votre token.
* La variable que vous utilisez pour parcourir sera `i`, du moins pour le tutoriel.
* Si vous trouvez un $ :
  * Assurez vous qu'il ne soit pas précédé d'un \\. Si c'est le cas, ignorez le.
  * Assurez vous qu'il ne soit pas suivi d'un whitespace. Si c'est le cas, ignorez le.
  * S'il est suivi d'un ? ... Ignorez ce cas pour l'instant, collez un bon gros `//TODO :` après le if, on y reviendra bien plus tard.
  * S'il est suivi d'un $ ... Ce n'est pas un cas a traiter. Mais vous pouvez le faire si vous le souhaitez ! $$ est remplacé par le PID du shell.
  * Sinon, considérez ceci :

![image](https://github.com/Nolan-du76/minishell/assets/8365167/66d0a226-3735-48f1-bf89-0c7d89dc6217)

  * Initialisez un int a 0. Ce dernier nous servira a déterminer combien de caractères compose le nom de la variable.
  * Continuez de parcourir le token, en utilisant le nouvel int, que l'on va nommer `j`. Ne modifiez pas `i`, on en aura besoin ! Si vous rencontrez un whitespace, la fin de chaine, ou certains caractères (Je vous laissez les chercher, mais sachez que ces cas fourbes ne seront pas testés pendant la correction ...), il s'agit de la fin de notre variable.
  * Extrayez la sous-chaine et sauvegardez la : on en aura besoin pour récupérér la valeur de la variable.
  * Utilisez get_env, envoyez lui votre sauvegarde : vous avez maintenant récupéré la valeur de la variable.
  * ça va piquer : Vous allez devoir maintenant insérer la valeur dans votre chaine, en remplaçant le nom ET le $.
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

## Details
