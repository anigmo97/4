;--------------------------------------------------------------------------------------------------------
;----------------------------------VARIABLES DIFUSAS-----------------------------------------------------
;--------------------------------------------------------------------------------------------------------
(deftemplate distancia 0 50 metros
    (
    (cerca (0 1) (15 0))
    (medio (10 0) (25 1) (35 1) (40 0))
    (lejos (35 0) (50 1))    
    )
)

(deftemplate velocidad_relativa -30 30 km/h
    (
    (alejando (-30 1) (0 0))
    (constante (-10 0) (0 1) (10 0))
    (acercando (0 0) (30 1))
    )
)

(deftemplate presion_freno 0 100 %
    (
    (nula (z 10 25))
    (media (PI 25 65))
    (alta (s 65 90))
    )
)

;--------------------------------------------------------------------------------------------------------
;----------------------------------  MODELO  ------------------------------------------------------------
;--------------------------------------------------------------------------------------------------------
(deftemplate vehiculo
    (slot id (type SYMBOL))
    (slot warning (type SYMBOL)(allowed-symbols ON OFF)(default OFF))
    (slot distance (type INTEGER))
    (slot velocity (type INTEGER))
    (slot presion (type FLOAT))
)

;--------------------------------------------------------------------------------------------------------
;----------------------------------  FUNCION FUSIFICACION------------------------------------------------
;--------------------------------------------------------------------------------------------------------
;              Convierte un valor real (crisp) por ejemplo 35 grados a un valor difuso caliente
(deffunction fuzzify (?fztemplate ?value ?delta)
    (bind ?low (get-u-from ?fztemplate))
    (bind ?hi (get-u-to ?fztemplate))
    (if (<= ?value ?low)
    then
        (assert-string (format nil "(%s (%g 1.0) (%g 0.0))" ?fztemplate ?low ?delta))
    else
        (if (>= ?value ?hi)
        then
            (assert-string (format nil "(%s (%g 0.0) (%g 1.0))" ?fztemplate (- ?hi ?delta) ?hi))
        else
            (assert-string
            (format nil "(%s (%g 0.0) (%g 1.0) (%g 0.0))" ?fztemplate (max ?low (- ?value ?delta)) 
                ?value (min ?hi (+ ?value ?delta)) ))
        ))) 


;--------------------------------------------------------------------------------------------------------
;------------------------  REGLAS CALCULO VALOR FUZZY PRESION_FRENO--------------------------------------
;--------------------------------------------------------------------------------------------------------
(defrule cerca-alejando
    (distancia cerca)
    (velocidad_relativa alejando)
=>
    (printout t "El coche delantero esta cerca pero se esta alejando... -> presion_freno Nula" crlf)
    (assert (presion_freno nula))
)

(defrule cerca-constante
    (distancia cerca)
    (velocidad_relativa constante)
=>
    (printout t "El coche delantero esta cerca y a velocidad constante -> presion_freno more-or-less Media" crlf)
    (assert (presion_freno more-or-less media))
)

(defrule cerca-acercando
    (distancia cerca)
    (velocidad_relativa acercando)
=>
    (printout t "El coche delantero esta cerca y se esta acercando... -> presion_freno Alta" crlf)
    (assert (presion_freno alta))
)

(defrule medio-alejando
    (distancia medio)
    (velocidad_relativa alejando)
=>
    (printout t "El coche delantero esta a distancia media pero se esta alejando... -> presion_freno Nula" crlf)
    (assert (presion_freno nula))
)

(defrule medio-constante
    (distancia medio)
    (velocidad_relativa constante)
=>
    (printout t "El coche delantero esta a distancia media y velocidad constante... -> presion_freno Nula" crlf)
    (assert (presion_freno nula))
)

(defrule medio-acercando
    (distancia medio)
    (velocidad_relativa acercando)
=>
    (printout t "El coche delantero esta a distancia media pero se esta acercando... -> presion_freno Media" crlf)
    (assert (presion_freno media))
)

(defrule lejos-alejando
    (distancia lejos)
    (velocidad_relativa alejando)
=>
    (printout t "El coche delantero esta a distancia lejana y se esta alejando... -> presion_freno Nula" crlf)
    (assert (presion_freno nula))
)

(defrule lejos-constante
    (distancia lejos)
    (velocidad_relativa constante)
=>
    (printout t "El coche delantero esta a distancia lejana y con velocidad constante... -> presion_freno nula" crlf)
    (assert (presion_freno nula))
)

(defrule lejos-acercando
    (distancia lejos)
    (velocidad_relativa acercando)
=>
    (printout t "El coche delantero esta a distancia media pero se esta acercando... -> presion_freno more-or-less Media" crlf)
    (assert (presion_freno more-or-less media))
)
;--------------------------------------------------------------------------------------------------------
;--------------------  REGLA ENCIENDE ALARMA SI PRESION = VERY ALTA  ------------------------------------
;--------------------------------------------------------------------------------------------------------
(defrule alarma 
    (declare (salience 100))
    (presion_freno very alta)
=>
    (assert (vehiculo (warning ON)))
    (printout t "La presion de frenado a ejercer es muy alta, se enciende la alarma" crlf)
)
;--------------------------------------------------------------------------------------------------------
;--------------------  REGLA CONVIERTE PRESION_FRENO A VALOR REAL (CRISP)--------------------------------
;--------------------------------------------------------------------------------------------------------
(defrule defuzzificar
    (declare (salience -1)) ;minima prioridad
    ?f <- (presion_freno ?)
=>  
    (bind ?x (maximum-defuzzify ?f))
    (bind ?y (moment-defuzzify ?f))
    (assert (vehiculo (presion ?x)))
    (printout t "La presion de frenado a ejercer será " ?x "% (maximum-defuzzify)" crlf)
    (printout t "La presion de frenado a ejercer será " ?y "% (moment-defuzzify)" crlf)
)
;--------------------------------------------------------------------------------------------------------
;----------------------------------FUNCION INICIO -------------------------------------------------------
;--------------------------------------------------------------------------------------------------------
(deffunction inicio ()
    (reset)
    (printout t "Introduzca la distancia del vehiculo" crlf)
    (bind ?distancia_crisp (read))
    (printout t "Introduzca la velocidad relativa con el vehiculo" crlf)
    (bind ?velocidad_crisp (read))
    (fuzzify distancia ?distancia_crisp 0.1)
    (fuzzify velocidad_relativa ?velocidad_crisp 0.1)
    (assert (vehiculo (distance ?distancia_crisp) (velocity ?velocidad_crisp)))
    (run)
)
