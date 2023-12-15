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
Cependant, ce split aura un defaut fatal : il ne prends pas en compte les " et ' !. Il faut alors recoder un split qui coupe la chaine en rencontrant un ;, mais que si ce dernier n'est pas encadre par des " ou '.
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
void  add_token_to_list(char *str, char quote, int has_space)
{

}

char *get_word(char *str, int *i)
{

}

t_word  parse(char *str)
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
      add_token_to_list
      i++;
  }
  
}
```
## Execution

## Details
