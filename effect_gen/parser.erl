-module(parser).

-export([check_lvl/1, align/3, newElement/1, endOfElement/1, addNewLines/1]).

check_lvl(Element) ->
    check_lvl(Element, 0).

check_lvl("    " ++ Rest, Lvl) ->
    check_lvl(Rest, Lvl+1);

check_lvl(_, Lvl) -> Lvl.

align(Text, Lvl, NewLvl) ->
    Paddad = shift(Text, NewLvl),

    if
        NewLvl > Lvl ->
            Paddad;

        NewLvl < Lvl ->
            Paddad ++ closeSkobes(Lvl, NewLvl);

        true ->
            Paddad ++ shift(["}"], NewLvl)
    end.

shift(List, Lvl) ->
    Pad = lists:duplicate(Lvl, "    "),
    [Pad ++ Line || Line <- List].


closeSkobes(Lvl, NewLvl) ->
    closeSkobes(Lvl, NewLvl, []).

closeSkobes(NewLvl, NewLvl, Rest) ->
    [shift_one("}", NewLvl) | Rest];

closeSkobes(Lvl, NewLvl, Rest) ->
    closeSkobes(Lvl-1, NewLvl, [shift_one("}", NewLvl) | Rest]).

newElement(Element) ->
    ["case " ++ Element ++ ":{"].

endOfElement(Lvl) ->
    [LastSkobe | Rest] = closeSkobes(Lvl, 0),
    [LastSkobe, "    break;" | Rest].

shift_one(Str, Lvl) ->
    lists:duplicate(Lvl, "    ") ++ Str.

addNewLines(List) ->
    [Str ++ "\n" || Str <- List].