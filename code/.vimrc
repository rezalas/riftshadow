version 6.x
set ttytype=ansi
syntax on

highlight Normal                  guibg=Black      guifg=Green 
highlight Cursor                  guibg=Grey70     guifg=White
highlight NonText                 guibg=Grey80
highlight StatusLine     gui=bold guibg=DarkGrey   guifg=Orange
highlight StatusLineNC            guibg=DarkGrey   guifg=Orange

highlight Comment    	 term=none       ctermfg=DarkBlue  guifg=#d1ddff
highlight Constant   	 term=none	  	 ctermfg=White      guifg=#ffa0a0
highlight Identifier 	 term=none	  	 ctermfg=Cyan       guifg=#40ffff
highlight Statement  	 term=none       ctermfg=DarkGreen  gui=bold		guifg=#ffff60
highlight PreProc    	 term=none       ctermfg=DarkBlue   guifg=#ff4500
highlight Type       	 term=none  ctermfg=DarkMagenta   gui=bold 		guifg=#7d96ff
highlight Special    	 term=none       ctermfg=DarkBlue 	guifg=Orange
highlight Ignore     	 ctermfg=black   guifg=bg
highlight Error      	 ctermfg=White   ctermbg=Red     	guifg=White    	guibg=Red
highlight Todo       	 ctermfg=Blue    ctermbg=Yellow  	guifg=Blue     	guibg=Yellow

highlight Float          ctermfg=White   guifg=#88AAEE
highlight Exception      ctermfg=Red     ctermbg=White   	guifg=Red		guibg=White
highlight Typedef        ctermfg=White   ctermbg=Blue    	gui=bold		guifg=White guibg=Blue
"highlight SpecialChar    ctermfg=DarkRed ctermbg=Black   	guifg=Black		guibg=White
highlight Delimiter      ctermfg=White   ctermbg=Black   	guifg=White		guibg=Black
highlight SpecialComment ctermfg=Black   ctermbg=Green   	guifg=Black		guibg=Green
highlight Label		 term=none     ctermfg=DarkGreen
set backspace=2

set scrolloff=2
set whichwrap=<,>,[,] 
set background=dark
set fileformat=dos
set formatoptions=tcq2
set mouse=a
set ruler
set shiftwidth=4
set showmatch
set tabstop=4
set textmode
set textwidth=80
set nowrap  
set ch=2  

map g G
map :X :x
map py set paste
map pn set nopaste
set cindent
set background=dark
