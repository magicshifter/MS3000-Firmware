(ns css.core
  (:require [garden.def :refer [defstylesheet defstyles]]
            [garden.units :refer [px em]]))

(defstyles typography
  [:body {:font-family "Ubuntu, \"Segoe UI\", \"Lucida Grande\", Verdana, sans-serif"
          :font-size (px 16)}]
  [:h1 :h2 :h3 :h4 :h5 :h6
   {:margin ".5em 0 1em"}]
  [:h1
   {:font-size "1.5em"}]
  [:h2
   {:font-size "1.3em"}]
  [:h3
   {:font-size "1.2em"}]
  [:h4
   {:font-size "1.1em"}]
  [:h5
   {:font-size "1em"}])

(defstyles screen
  (-> typography))