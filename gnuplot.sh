#!/bin/bash
trap exit ERR

# argumentos
# ----------
if [ -z "${1}" ]; then
  EXT="png"
else
  EXT="${1}"
fi

# funcoes
# ----------------
die() {
  # imprime uma mensagem a finaliza a execução do script
  # argumentos
  # $@ = mensagem a ser impressa antes de finalizar o script
  echo -e "$@"; exit 1
}

plot() {
  # plota o gráfico para um dado arquivo de entrada
  # argumentos
  # $1 = codigo do algoritmo [1-7]
  # $2 = ordem dos dados [asc,desc,rand,part]
  # $3 = família de funções correspondente à complexidade de tempo do algoritmo (ex: "a*x + b")
  # $4 = variáveis a serem usadas para o fit (ex: "a,b")
  # $5 = formato de T(n) para impressão no título (ex: "T(n) = %.4gn + %.4g")
  # $6 = unidade de T(32) (ex: "horas", "minutos")
  # $7 = denominador de T(32) (ex: "3600" -> horas, "60" -> minutos)

  INPUT="'./data/out_${1}_${2}.csv'"

# case ${1} in
#   1) TITLE="Insertion" ;;
#   2) TITLE="Merge"     ;;
#   3) TITLE="Heap"      ;;
#   4) TITLE="Quick"     ;;
#   5) TITLE="Counting"  ;;
#   6) TITLE="Radix"     ;;
#   7) TITLE="Bucket"    ;;
# esac

  case ${2} in
    asc)  COLOR="#268bd2";TITLE="Crescente"      ;; #TITLE+=" - Crescente"      ;;
    desc) COLOR="#dc322f";TITLE="Decrescente"    ;; #TITLE+=" - Decrescente"    ;;
    rand) COLOR="#b58900";TITLE="Aleatório"      ;; #TITLE+=" - Aleatório"      ;;
    part) COLOR="#859900";TITLE="10%% Crescente" ;; #TITLE+=" - 10%% Crescente" ;;
  esac

  gnuplot << EndOfFile
  set datafile separator ","
  set encoding utf8
  set terminal ${EXT} enhanced font 'Arial,12' size 500,500 background rgb 'white'
  set output "plots/plot_${1}_${2}.${EXT}"

  set xlabel "Tamanho"
  set ylabel "Tempo (ns)"
  set key left top
  set xrange [0:]
  set yrange [0:]
  set xtics nomirror
  set ytics nomirror
  set format y "%.1s*10^{%S}" # notação científica no eixo y
  set grid

  set fit logfile "/dev/null"
  set fit quiet

  stats ${INPUT} nooutput

  t(x) = ${3}
  fit t(x) ${INPUT} via ${4}

  set title sprintf("${TITLE}\n${5}\nEstimativa de T(2^{32}): %.2f ${6}", ${4}, t(2**32)/((10**9)*(${7})))

  # ssd_y / sqrt(records) representa o erro padrão de y
  plot ${INPUT} u 1:2:(STATS_ssd_y/(sqrt(STATS_records))) t "medições" w yerrorbars lc rgb "#1d1f21",\
       t(x) title "T(n)" lc rgb "${COLOR}" lt 1 lw 2


EndOfFile
}

# início do script
# ----------------
type gnuplot > /dev/null 2>&1 || die "gnuplot nao foi encontrado no PATH."

[ -d ./plots ] || mkdir ./plots

plot 1 "asc"  "a*x + b"          "a,b"   "T(n) = %.4gn + %.4g"           "segundos" "1"
plot 1 "desc" "a*x**2 + b*x + c" "a,b,c" "T(n) = %.4gn^2 + %.4gn + %.4g" "anos"     "3600*24*365"
plot 1 "rand" "a*x**2 + b*x + c" "a,b,c" "T(n) = %.4gn^2 + %.4gn + %.4g" "anos"     "3600*24*365"
plot 1 "part" "a*x**2 + b*x + c" "a,b,c" "T(n) = %.4gn^2 + %.4gn + %.4g" "anos"     "3600*24*365"

plot 2 "asc"  "a*x*log(x) + b"   "a,b"   "T(n) = %.4gn*log(n) + %.4g"    "minutos"  "60"
plot 2 "desc" "a*x*log(x) + b"   "a,b"   "T(n) = %.4gn*log(n) + %.4g"    "minutos"  "60"
plot 2 "rand" "a*x*log(x) + b"   "a,b"   "T(n) = %.4gn*log(n) + %.4g"    "minutos"  "60"
plot 2 "part" "a*x*log(x) + b"   "a,b"   "T(n) = %.4gn*log(n) + %.4g"    "minutos"  "60"

plot 3 "asc"  "a*x*log(x) + b"   "a,b"   "T(n) = %.4gn*log(n) + %.4g"    "minutos"  "60"
plot 3 "desc" "a*x*log(x) + b"   "a,b"   "T(n) = %.4gn*log(n) + %.4g"    "minutos"  "60"
plot 3 "rand" "a*x*log(x) + b"   "a,b"   "T(n) = %.4gn*log(n) + %.4g"    "minutos"  "60"
plot 3 "part" "a*x*log(x) + b"   "a,b"   "T(n) = %.4gn*log(n) + %.4g"    "minutos"  "60"

plot 4 "asc"  "a*x**2 + b*x + c" "a,b,c" "T(n) = %.4gn^2 + %.4gn + %.4g" "anos"     "3600*24*365"
plot 4 "desc" "a*x**2 + b*x + c" "a,b,c" "T(n) = %.4gn^2 + %.4gn + %.4g" "anos"     "3600*24*365"
plot 4 "rand" "a*x*log(x) + b"   "a,b"   "T(n) = %.4gn*log(n) + %.4g"    "minutos"  "60"

# FIXME na análise teórica concluímos que a complexidade de quicksort para entradas parcialmente
# ordenadas era logarítmica, porém na análise prática foi observado que continua quadrática.

#plot 4 "part" "a*x*log(x) + b"   "a,b"   "T(n) = %.4gn*log(n) + %.4g"    "horas"    "3600"
plot 4 "part" "a*x**2 + b*x + c" "a,b,c" "T(n) = %.4gn^2 + %.4gn + %.4g" "anos"     "3600*24*365"

plot 5 "asc"  "a*x + b"          "a,b"   "T(n) = %.4gn + %.4g"           "segundos" "1"
plot 5 "desc" "a*x + b"          "a,b"   "T(n) = %.4gn + %.4g"           "segundos" "1"
plot 5 "rand" "a*x + b"          "a,b"   "T(n) = %.4gn + %.4g"           "segundos" "1"
plot 5 "part" "a*x + b"          "a,b"   "T(n) = %.4gn + %.4g"           "segundos" "1"

plot 6 "asc"  "a*x + b"          "a,b"   "T(n) = %.4gn + %.4g"           "minutos" "60"
plot 6 "desc" "a*x + b"          "a,b"   "T(n) = %.4gn + %.4g"           "minutos" "60"
plot 6 "rand" "a*x + b"          "a,b"   "T(n) = %.4gn + %.4g"           "minutos" "60"
plot 6 "part" "a*x + b"          "a,b"   "T(n) = %.4gn + %.4g"           "minutos" "60"

plot 7 "asc"  "a*x + b"          "a,b"   "T(n) = %.4gn + %.4g"           "minutos"  "60"
plot 7 "desc" "a*x + b"          "a,b"   "T(n) = %.4gn + %.4g"           "minutos"  "60"
plot 7 "rand" "a*x + b"          "a,b"   "T(n) = %.4gn + %.4g"           "minutos"  "60"
plot 7 "part" "a*x + b"          "a,b"   "T(n) = %.4gn + %.4g"           "minutos"  "60"

echo "gráficos gerados no diretório 'plots'."
