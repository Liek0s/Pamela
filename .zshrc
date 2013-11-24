
#ENV
export PATH="/home/jack/share/Dropbox/share/scripts/:/usr/lib/colorgcc/bin:$PATH"
export EDITOR='emacs -nw'
export PAGER='/usr/bin/most -s'

##HISTORY
setopt histignorealldups sharehistory
HISTSIZE=1000
SAVEHIST=1000

#KEYBINDINGS
bindkey -e
bindkey ';5D' emacs-backward-word
bindkey ';5C' emacs-forward-word


#OPTIONS
zmodload -a colors
autoload colors && colors
setopt AUTO_CD

#PROMPT

local curdir="%d"
local hostname="%M"
local username="%n" 
local bold="%B"
local unbold="%b"
local standout="%S"
local unstandout="%s"
local colorfg="%F"
local uncolorfg="%f"
local colorbg="%K"
local uncolorbg="%k"
local mytime="%*"
local mydate="%D"
local line_tty="%y"
local rootorwhat="%#"
local return_status="%?"

PROMPT="$colorfg{blue}$uncolorfg$bold$curdir$unbold$colorfg{blue}$uncolorfg
$username$bold$colorfg{red}:${uncolorfg}$unbold$hostname $colorfg{red}->${uncolorfg} $colorfg{$uncolorfg "

RPROMPT="$mytime"

#ALIAS

alias ls="ls --color=auto"
alias ll="ls -la"
alias la="ls -a"

alias emacs="emacs -nw"
alias ne='emacs'
alias py='python3'

#AUTOCOMPLETE

autoload -Uz compinit
compinit

zstyle ':completion:*' auto-description 'specify: %d'
zstyle ':completion:*' completer _expand _complete _correct _approximate
zstyle ':completion:*' format 'Completing %d'
zstyle ':completion:*' group-name ''
zstyle ':completion:*' menu select=2 eval "$(dircolors -b)"
zstyle ':completion:*' list-colors ${(s.:.)LS_COLORS}
zstyle ':completion:*' list-colors 'reply=( "=(#b)(*$VAR)(?)*=00=$color[green]=$color[bg-green]" )'
zstyle ':completion:*:*:*:*:hosts' list-colors '=*=31;41'
zstyle ':completion:*:*:*:*:users' list-colors '=*=$color[green]=$color[blue]'
zstyle ':completion:*' matcher-list '' 'm:{a-z}={A-Z}' 'm:{a-zA-Z}={A-Za-z}' 'r:|[._-]=* r:|=* l:|=*'
zstyle ':completion:*' select-prompt %SScrolling active: current selection at %p%s
zstyle ':completion:*' use-compctl false
zstyle ':completion:*' verbose true
zstyle ':completion:*:*:kill:*:processes' list-colors '=(#b) #([0-9]#)*=0=01;31'
zstyle ':completion:*:kill:*' command 'ps -u $USER -o pid,%cpu,tty,cputime,cmd'

#END
