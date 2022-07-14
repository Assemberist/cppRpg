-module(builder).

-export([build/0]).

build() ->
    Files = filelib:wildcard("*.eff"),
    {ok, OutFile} = file:open("effect_calculator.cpp", [append]),
    Writer = fun(X) -> file:write(OutFile, X) end,
    lists:map(Writer,
        lists:map(fun parse_file/1,
            Files)).

parse_file(FileName) ->
    {ok, Entry} = file:read_file(FileName),
    Elements = string:split(binary_to_list(Entry), "\n", all),
    analyse(Elements).

analyse([Head | Tail]) ->
    analyse(Head, 0, Tail, []).

analyse(_, Lvl, [], Ready) ->
    Rest = parser:endOfElement(Lvl),
    RestEnded = parser:addNewLines(Rest),
    lists:merge(lists:reverse(RestEnded ++ Ready));

analyse(Element, Lvl, [Head | Tail], Ready) ->
    NewLvl = parser:check_lvl(Head),
    
    case NewLvl of
        0 when Lvl == 0 -> 
            NewElement = Head,
            NewText = parser:newElement(Head);

        0 ->
            NewElement = Element,
            NewText = parser:endOfElement(Lvl);

        _ ->
            NewElement = Element,
            NotAligned = parse(Head),
            NewText = parser:align(NotAligned, Lvl, NewLvl)
    end,

    analyse(NewElement, NewLvl, Tail, NewText ++ Ready).

parse(Head)-> Head.