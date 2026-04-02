APLICATIE BILETE CFR

Prin aplicatia mea, utilizatorii pot cumpara bilete de calatorie printr-o retea de trenuri, formata din rute si trenurile propriu-zise.
Utilizatorul introduce rutele dorite, trenurile care le populeaza, orele de la care pleaca, vagoanele care sunt alcatuite.
Pentru a cumpara un bilet se introduc statia de pornire si cea de sosire, si aplicatia returneaza o ruta sugerata pentru a ajunge,
in functie de trenurile valabile.
Meniul permite vizualizarea biletelor cumparate si a retelei de trenuri, si actualizarea si stergerea rutelor.


Sunt valabile 3 tipuri de vagoane(clasa 1, clasa 2, cuseta) si 4 tipuri de bilet(intreg, tineret, pensionar, CFR+ membership)


Pentru a stabili ruta propusa am implementat un graf neorientat pe care aplic o variatiune a algoritmului Breadth First Search.
Biletele sunt structurate in functie de cate schimbari este nevoie pentru a ajunge la destinatie: pentru fiecare ruta care trebuie folosita este salvata cate o 'calatorie' care contine date relevante acelui tren.



Precizari si Limitari:

-voi reveni cu o optiune de a preseta reteaua de trenuri din meniu

-locurile sunt stabilite aleator din pacate!

-orele de plecare trebuie introduse in minute relativ cu 00:00, si sunt la fel pentru ambele sensuri de mers ale trenului (din lipsa de timp, pot reveni pana luni)

-algoritmul proiectat pentru stabilirea rutei propune o solutie eficienta din punct de vedere al distantei si al numarului de schimbari necesare, dar presupune o alegere convenabila a orelor de plecare pentru trenuri.

-pe viitor as dori sa implementez o interfata grafica, introducerea rapida a datelor in sistem folosind fisiere json, si un algoritm mai complex folosind Djisktra's algorithm!

<img width="334" height="334" alt="image" src="https://github.com/user-attachments/assets/5d8571d9-636e-424d-87e0-771d493ced3d" />


