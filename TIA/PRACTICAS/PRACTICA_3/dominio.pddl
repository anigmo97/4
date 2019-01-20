(define (domain ejemplo1)
    (:requirements
        :durative-actions
        :typing
        :fluents)
    (:types
        driver
        truck
        city
        package
        - object)
    (:predicates
        (at ?x - (either truck driver package) ?c - city) ; un camion un paquete o un conductor puede estar en una ciudad
        (in ?p - (either package driver) ?t - truck)
        (existe-camino ?c1 ?c2 - city))
    (:functions
        (time ?c1 ?c2 - city)
        (cost ?c1 ?c2 - city)
        (time-bus)
        (cost-bus)

        ; cargar/descargar paquetes ligeros
        (time-load-package)
        (cost-load-package)
        (time-unload-package)
        (cost-unload-package)

        ; cargar/descargar paquetes pesados
        (time-load-heavy-package)
        (cost-load-heavy-package)
        (time-unload-heavy-package)
        (cost-unload-heavy-package)

        (time-get-on)
        (cost-get-on)
        (time-get-off)
        (cost-get-off)
        (coste-total))

    ; Actions
    (:durative-action drive
        :parameters (?t - truck ?d - driver ?origin ?dest - city)
        :duration (= ?duration (time ?origin ?dest))
        :condition (and
            (over all (in ?d ?t))
            (over all (existe-camino ?origin ?dest))
            (at start (at ?t ?origin)))
        :effect (and
            (at start (not (at ?t ?origin)))
            (at end   (at ?t ?dest))
            (at end   (increase (coste-total) (cost ?origin ?dest))))
    )
    (:durative-action load-heavy-package
        :parameters (?p - package ?t - truck ?c - city)
        :duration (= ?duration (time-load))
        :condition (and
            (at start (at ?p ?c))
            (at start (at ?t ?c)))
        :effect (and
            (at start (not (at ?p ?c)))
            (at end (in ?p ?t))
            (at end (increase (coste-total) (cost-load))))
    )
    (:durative-action load-package
        :parameters (?p - package ?t - truck ?c - city)
        :duration (= ?duration (time-load))
        :condition (and
            (at start (at ?p ?c))
            (at start (at ?t ?c)))
        :effect (and
            (at start (not (at ?p ?c)))
            (at end (in ?p ?t))
            (at end (increase (coste-total) (cost-load))))
    )
    (:durative-action unload
        :parameters (?p - package ?t - truck ?c - city)
        :duration (= ?duration (time-unload))
        :condition (and
            (at start (in ?p ?t))
            (at start (at ?t ?c)))
        :effect (and
            (at start (not (in ?p ?t)))
            (at end (at ?p ?c))
            (at end (increase (coste-total) (cost-unload))))
    )
    (:durative-action get-on
        :parameters (?d - driver ?t - truck ?c - city)
        :duration (= ?duration (time-get-on))
        :condition (and
            (at start (at ?t ?c))
            (at start (at ?d ?c)))
        :effect (and
            (at start (not (at ?d ?c)))
            (at end (in ?d ?t))
            (at end (increase (coste-total) (cost-get-on))))
    )
    (:durative-action get-off
        :parameters (?d - driver ?t - truck ?c - city)
        :duration (= ?duration (time-get-off))
        :condition (and
            (at start (at ?t ?c))
            (at start (in ?d ?t)))
        :effect (and
            (at start (not (in ?d ?t)))
            (at end (at ?d ?c))
            (at end (increase (coste-total) (cost-get-off))))
    )
    (:durative-action take-bus
        :parameters (?d - driver ?origin ?dest - city)
        :duration (= ?duration (time-bus))
        :condition (and
            (at start (at ?d ?origin))
            (over all (existe-camino ?origin ?dest)))
        :effect (and
            (at start (not (at ?d ?origin)))
            (at end (at ?d ?dest))
            (at end (increase (coste-total) (cost-bus))))
    )
)