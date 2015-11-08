(ns magic-ring.core
  (:require [goog.dom :as gdom]
            [om.next :as om :refer-macros [defui]]
            [sablono.core :as html :refer-macros [html]]
            [magic-ring.components.root :as root]))

(root/init)