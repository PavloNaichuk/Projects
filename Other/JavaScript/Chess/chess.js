let map = Array();
let info  = Array();

let moveColor = "white";
let moveFromX;
let moveFromY;

function initMap()
{
    map =
        [
            ["R", "P", " ", " ", " ", " ", "p", "r" ],
            ["N", "P", " ", " ", " ", " ", "p", "n" ],
            ["B", "P", " ", " ", " ", " ", "p", "b" ],
            ["Q", "P", " ", " ", " ", " ", "p", "q" ],
            ["K", "P", " ", " ", " ", " ", "p", "k" ],
            ["B", "P", " ", " ", " ", " ", "p", "b" ],
            ["N", "P", " ", " ", " ", " ", "p", "n" ],
            ["R", "P", " ", " ", " ", " ", "p", "r" ],
        ];
}
function initInfo()
{
    info =
        [
            [" ", " ", " ", " ", " ", " ", " ", " " ],
            [" ", " ", " ", " ", " ", " ", " ", " " ],
            [" ", " ", " ", " ", " ", " ", " ", " " ],
            [" ", " ", " ", " ", " ", " ", " ", " " ],
            [" ", " ", " ", " ", " ", " ", " ", " " ],
            [" ", " ", " ", " ", " ", " ", " ", " " ],
            [" ", " ", " ", " ", " ", " ", " ", " " ],
            [" ", " ", " ", " ", " ", " ", " ", " " ],
        ];
}

function markMovesFrom()
{
    initInfo();
    for (let x = 0; x < 8; ++x)
    {
        for (let y = 0; y < 8; ++y)
        {
            if (canMoveFrom(x, y))
                info[x][y] = 1;
        }
    }
}
function markMovesTo()
{
    initInfo();
    for (let x = 0; x < 8; ++x)
    {
        for (let y = 0; y < 8; ++y)
        {
            if (canMoveTo(x, y))
                info[x][y] = 2;
        }
    }
}

function canMoveFrom(x, y)
{
    return (getColor(x, y) == moveColor);
}

function canMoveTo(x, y)
{
    if (map[x][y] == " ")
        return true;
    return (getColor(x, y) != moveColor);
}

function getColor(x, y)
{
    let figure  = map[x] [y];
    if (figure == " ")
        return "";
    return (figure.toUpperCase() == figure) ? "white" : "black";
}


function clickBox(x, y)
{
    if (info[x][y] == "1")
        clickBoxFrom(x, y);
    if (info[x][y] == "2")
        clickBoxTo(x, y);
}

function clickBoxFrom(x, y)
{
    moveFromX = x;
    moveFromY = y;
    markMovesTo();
    showMap();
}
function clickBoxTo(x, y)
{
    map[x][y] = map[moveFromX][moveFromY];
    map[moveFromX][moveFromY] = " ";
    turnMove();
    markMovesFrom();
    showMap();
}

function turnMove()
{
    moveColor = moveColor == "white" ? "black" : "white";
}

function figureToHtml(figure)
{
    switch(figure)
    {
        case "K" : return "&#9812;"; case "k" : return "&#9818;";
        case "Q" : return "&#9813;"; case "q" : return "&#9819;";
        case "R" : return "&#9814;"; case "r" : return "&#9820;";
        case "B" : return "&#9815;"; case "b" : return "&#9821;";
        case "N" : return "&#9816;"; case "n" : return "&#9822;";
        case "P" : return "&#9817;"; case "p" : return "&#9823;";
        default : return " ";

    }
}

function showMap()
{
    let html = "<table border= '1' cellpadding='2', cellpacing='0'>";
    for(let y = 7; y >= 0; --y)
    {
        html += "<tr>";
        html += "<td>" + (y  + 1) + "</td>";
        for (let x = 0; x < 8; ++x)
        {
            let color;
            if (info[x][y] == " ")
                color = (x + y) % 2 ? "#eeffee" : "#abcdef";
            else
                color = info[x][y] == "1" ? "#aaffaa" : "ffaaaa";
            html += "<td style='height: 50px; width: 50px; " +
                "background-color: " + color + ";" +
                " text-align: center;" +
                "font-size: 40px;" +
                " color: #223377;" +
                " ' onclick='clickBox(" + x + ", " + y + ");'>";

            html+= figureToHtml(map[x][y]);
            html+="</td>";
        }
        html += "</tr>";
    }
    document.getElementById("chess").innerHTML = html;
}

function start()
{
    initMap();
    markMovesFrom();
    showMap();
}
start();