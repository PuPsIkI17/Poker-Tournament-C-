# Poker-Tournament-C-
Pislari Vadim -> Facultatea Automatica si Calculatoare -> Universitatea Politehnica Bucuresti

                                                 (Romanian language)
                                               -------DESCRIERE-------
 Turneele de poker se pot organiza cu un număr aproape nelimitat de jucători. Cu toate acestea, organizatorii au sarcina de a stabili un algoritm prin care aceștia să fie împărțiți la mese. Mai mult, pe parcursul turneului, jucătorii pot pierde jocul și trebuie să-și elibereze locul, care poate fi folosit de un alt jucător. Iar pentru a păstra jocul de cărți interesant, trebuie să existe cât mai mulți jucători la o masă. De aceea organizatorii pot decide închiderea unei mese și redistribuirea jucătorilor acesteia la celelalte mese. Proiectul presupune realizarea unui program care să ajute la organizarea și așezarea jucătorilor la un turneu de poker.
 
 Sala de joc este reprezentată sub forma unei liste simplu înlănțuite fără santinelă (nu este circulară), conținând elemente de tip TMasa. O masa de joc este reprezentată sub forma unei liste circulare simplu înlănțuite cu santinelă, a cărei structură a fost menționată și mai sus. Aceasta va avea în componență numele mesei (char *), numărul curent de jucători (int), numărul maxim de jucători (int) și o listă de elemente de tip jucător. 

Jucătorul este definit printr-un id de identificare (char *) și un număr de mâini pe care le mai poate juca la momentul curent (int). Listele utilizate sunt GENERICE. 

Evenimentele ce pot avea loc asupra sălii/meselor/jucătorilor sunt: 
  
a) print 
</br>-Evenimentul prin care se va face afișarea sălii la momentul curent. Jucătorii de la masă sunt afișați in ordinea mâinii de joc curente(configurația la momentul actual).            
</br>-Jucătorul care se află “primul” la momentul curent este chiar cel din dreapta santinelei. 
</br>-În cazul în care nu există nicio masă, se afiseaza mesajul "Sala este inchisa!"

b) noroc [nume_masă] [nume_jucător] [grad_noroc] 
</br>-Evenimentul prin care un jucător primește mai multe șanse de a rămâne în joc, numărul de mâini pe care acesta le poate juca fiind incrementat. 
</br>-Parametrii reprezintă: ● noroc - numele operației ● nume_masa - numele mesei la care jucătorul se află ● nume_jucător - numele jucătorului “norocos” ● grad_noroc - întreg mai mare ca 0 

c) ghinion [nume_masă] [nume_jucător] [grad_ghinion] 
</br>-Este evenimentul opus precedentului (noroc), prin care unui jucător i se scade  din numărul de mâini pe care le poate juca valoarea grad_ghinion. 
</br>-De asemenea, se verifica pentru ambele evenimente (noroc, ghinion) faptul că datele sunt valide.
Asta înseamnă că:
</br>- În cazul în care numele mesei nu este valid ([nume_masă] nu apare în lista de mese) se va afișa mesajul: “​Masa [nume_masa] nu exista!” 
</br>- În cazul în care numele jucătorului nu este valid ([nume_jucător] nu apare în lista mesei [nume_masa]) se va afișa mesajul: “Jucatorul [nume_jucător] nu exista la masa [nume_masa]!” 

d) tura [nume_masă]  
</br>-Evenimentul prin care se informează că a avut loc o mână jucată la o anumită masa. Acest lucru implică decrementarea cu 1 a valorii din câmpului număr de mâini pentru fiecare jucător de la masa respectivăși reordonarea în listă conform regulii: al doilea jucător (al doilea element din lista) devine primul; al treilea devine al doilea; primul jucător devine ultimul. Rotația are loc în jurul santinelei. 
</br>-Realizarea unei ture presupune mai întâi rotația în jurul santinelei și apoi efectuarea decrementării numărului de mâini pentru fiecare jucător de la masă. 

e) tura_completa 
</br>-Evenimentul are un comportament similar celui descris mai sus, însă se realizeaza pentru toate mesele. 
</br>-În cazul în care în urma unui eveniment, un jucător ajunge să aibă valoarea numărului de mâini pe care le poate juca mai mică sau egală cu 0, jucătorul este eliminat de la masa (înregistrarea sa este din lista). 
</br>-În cazul în care masa are un număr curent de jucători egal cu 0, ea este eliminată și ștearsă din memorie.
 
f) clasament [nume_masă] 
</br>-Prin apelarea acestui eveniment, se afiseaza jucătorii de la o anumită masă, descrescător după numărul de mâini rămase pentru a fi jucate de fiecare jucător. Lista este construită sortată descrescător în funcție de clasamentul jucătorilor, prin inserarea ordonata, succesiva jucătorilor. 
</br>- În cazul în care doi jucători au același număr de mâini rămase pentru a fi jucate, compararea dintre cei doi se face lexicografic, in funcție de numele acestora, folosind ordinea din tabela ASCII. 

g) inchide [nume_masa] 
Apelând evenimentul “închide nume_masa”, se dorește desființarea (ștergerea) mesei primita ca parametru. 
</br>-Altfel, lista destinată acelei mese va fi ștearsă din memorie, iar jucătorii aflați la masa respectiva sunt redistribuiți la mese după cum urmează: 
</br>-Se parcurg mesele de la început spre sfârșit. Dacă masa curentă dispune de locuri libere, sunt adăugați atâția jucători de la masa eliminată încât noua masă permite, în ordinea în care aceștia se aflau la masa închisă. Jucătorii sunt adaugati la sfârșitul listei. Procedeul se repetă, continuând la următoarea masă, până când toți jucătorii de la masa eliminată sunt așezați la o noua masă 
