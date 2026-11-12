   // emulateur/include/mesure.h
   #pragma once
   #include <stdint.h>

   // Hauteur signalant une mesure invalide (capteur muet, trou de donnée).
   // Alignée sur le contrat d'interface : water_mm = 0xFFFF = invalide.
   static const uint16_t MESURE_INVALIDE = 0xFFFF;

   struct Mesure {
       uint32_t t_s;          // temps écoulé depuis le début du scénario, en s
       uint32_t horodatage;   // date de la mesure, en secondes epoch UTC
       uint16_t hauteur_mm;   // hauteur d'eau en mm ; MESURE_INVALIDE si non valide

       bool valide() const { return hauteur_mm != MESURE_INVALIDE; }
   };