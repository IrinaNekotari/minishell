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

## Decomposition (Parsing)

## Execution

## Details
