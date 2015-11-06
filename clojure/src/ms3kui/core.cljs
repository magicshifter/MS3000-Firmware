(ns ms3kui.core
  (:require [goog.dom :as gdom]
            [om.next :as om :refer-macros [defui]]
            [sablono.core :as html :refer-macros [html]]
            [ms3kui.components.root :as root]))

(om/add-root! root/reconciler root/component (gdom/getElement "app"))