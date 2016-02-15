%Submitted by: Naveen Thomas(UIN: 423008937)

% Intersection of 2 lists[PROBLEM 1]

intersection([],_,[]).
intersection([A|C], B, [A|X]) :- member(A,B), intersection(C,B,X).
intersection([_|C], B, X) :- intersection(C, B, X).


% Meal Planning[PROBLEM 2]
% Sources of calories
calories(water, 0).
calories(hamburger, 354).
calories(carrot, 25).
calories(salad, 100).
calories(banana, 105).
calories(apple, 95).
calories(peanuts, 828).
calories(chicken_soup, 87).
calories(lasagna, 166).
calories(apple_pie, 67).
calories(beans, 41).
calories(peas, 118).
calories(milk, 8).
calories(orange_juice, 39).
calories(coke, 140).
calories(cookie, 132).
calories(naan, 78).
calories(potato_soup, 149).
calories(carrot, 50).
calories(coffee, 50).

%Legumes
legumes(peas).
legumes(beans).
legumes(peanuts).

%Vegetable
vegetable(carrot).
vegetable(potato_soup).
vegetable(salad).
vegetable(X) :- legumes(X).

%Fruit
fruit(orange_juice).
fruit(apple_pie).
fruit(apple).
fruit(banana).

%meat
meat(hamburger).
meat(lasagna).
meat(chicken_soup).

%drink
drink(water).
drink(orange_juice).
drink(milk).
drink(coke).
drink(coffee).

% Sources of proteins
protein(hamburger).
protein(chicken_soup).
protein(lasagna).
protein(potato_soup).
protein(banana).
protein(peas).
protein(beans).
protein(peanuts).
protein(milk).


contain_Proteins([A|X]) :- protein(A). 
contain_Proteins([A|X]) :- contain_Proteins(X). 

contain_fruit_or_Vegetable(X) :- contain_Fruit(X). 
contain_fruit_or_Vegetable(X) :- contain_Vegetable(X). 
contain_Fruit([A|X]) :- fruit(A). 
contain_Fruit([A|X]) :- contain_Fruit(X). 
contain_Vegetable([A|X]) :- vegetable(A). 
contain_Vegetable([A|X]) :- contain_Vegetable(X). 

findCalories([],0).
findCalories([H|T],Y) :- calories(H, A), findCalories(T, B), Y is A+B. 

vegetarian([]).
vegetarian([A|X]) :- \+ meat(A), vegetarian(X).

good_meal(X, CALORIES) :- contain_Proteins(X), contain_fruit_or_Vegetable(X), findCalories(X, Y), Y>=400, Y=<600, CALORIES is Y.
good_meal_vegetarian(X, CALORIES) :- good_meal(X, CALORIES), vegetarian(X).
good_meal_non_vegetarian(X, CALORIES) :- good_meal(X, CALORIES), \+ vegetarian(X).

% good_meal([salad, banana, water, apple_pie, beans, milk, orange_juice, carrot], X)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Calculating square root[PROBLEM 3]
sqrt(IN, LOW, HIGH, OUT, PRECISION) :- VAL is (LOW+HIGH)/2, VAL*VAL<IN, abs(VAL*VAL-IN)>PRECISION, sqrt(IN,VAL, HIGH, OUT, PRECISION).
sqrt(IN, LOW, HIGH, OUT, PRECISION) :- VAL is (LOW+HIGH)/2, VAL*VAL>IN, abs(VAL*VAL-IN)>PRECISION, sqrt(IN,LOW, VAL, OUT, PRECISION).
sqrt(IN, LOW, HIGH, OUT, PRECISION) :- VAL is (LOW+HIGH)/2, abs(VAL*VAL-IN)=<PRECISION, OUT is VAL.
sqrt(IN,OUT) :- sqrt(IN, 0, IN, OUT, 0.0001). 

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 5 queens problem[PROBLEM 4]
assignValues(A,B,C,D,E,[H|T]) :- A is H, assignValues(B,C,D,E,T). 
assignValues(B,C,D,E,[H|T]) :- B is H, assignValues(C,D,E,T). 
assignValues(C,D,E,[H|T]) :- C is H, assignValues(D,E,T). 
assignValues(D,E,[H|T]) :- D is H, assignValues(E,T). 
assignValues(E,[H|T]) :- E is H. 

isConsistent([]).
isConsistent([H|T]) :- checkConsistency(H,T,1), isConsistent(T).

checkConsistency(A,[],_).
checkConsistency(A,[H|T],D) :- A \= H, V is abs(A-H), V \= D, Y is D+1, checkConsistency(A,T,Y). 

queens(A,B,C,D,E) :- permute([1,2,3,4,5],X), valid(X,A,B,C,D,E).
valid(X,A,B,C,D,E) :- isConsistent(X), assignValues(A,B,C,D,E,X).  

takeout(X,[X|R],R).  
takeout(X,[F |R],[F|S]) :- takeout(X,R,S).

permute([X|Y],Z) :- permute(Y,W), takeout(X,Z,W).  
permute([],[]).
