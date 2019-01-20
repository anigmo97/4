(define (problem ejemplo1caso1)
    (:domain ejemplo1) ; nombre del dominio – debe corresponderse con el definido en el dominio
    (:objects
        A - city
        B - city
        C - city
        D - city
        E - city

        p1 - package
        p2 - package

        c1 - truck
        c2 - truck

        d1 - driver
        d2 - driver)
    (:init
        (at p1 A) ; el paquete 1 esta en la ciudad A – información proposicional
        (at p2 D)
        (at c1 A)
        (at c2 A)
        (at d1 C)
        (at d2 D)

        (= (time A B) 4)
        (= (cost A B) 1)
        (= (time B A) 4)
        (= (cost B A) 1)

        (= (time A C) 2)
        (= (cost A C) 1)
        (= (time C A) 2)
        (= (cost C A) 1)
        
        (= (time B C) 3)
        (= (cost B C) 2)
        (= (time C B) 3)
        (= (cost C B) 2)
        
        (= (time B D) 3)
        (= (cost B D) 3)
        (= (time D B) 3)
        (= (cost D B) 3)
        
        (= (time B E) 4)
        (= (cost B E) 3)
        (= (time E B) 4)
        (= (cost E B) 3)
        
        (= (time C E) 9)
        (= (cost C E) 6)
        (= (time E C) 9)
        (= (cost E C) 6)

        (existe-camino A B)
        (existe-camino A C)
        (existe-camino B A)
        (existe-camino B C)
        (existe-camino B D)
        (existe-camino B E)
        (existe-camino C A)
        (existe-camino C B)
        (existe-camino C E)
        (existe-camino D B)
        (existe-camino E B)
        (existe-camino E C)

        ; cargar/descargar paquetes ligeros
        (= (time-load-package) 1)
        (= (cost-load-package) 2)
        (= (time-unload-package) 1)
        (= (cost-unload-package) 2)

        ; cargar/descargar paquetes pesados
        (= (time-load-heavy-package) 2)
        (= (cost-load-heavy-package) 4)
        (= (time-unload-heavy-package) 2)
        (= (cost-unload-heavy-package) 4)

        ;subir/bajar 
        (= (time-get-on) 1)
        (= (cost-get-on) 1)
        (= (time-get-off) 1)
        (= (cost-get-off) 1)
        
        (= (time-bus) 10)
        (= (cost-bus) 3)

        (= (coste-total) 0)
    )
    (:goal (and
        (at p1 E)
        (at p2 C)
        (at c2 A)
        (at d1 B)
    ))
    (:metric minimize (+ (* 0.2 (total-time)) (* 0.5 (coste-total))))
)