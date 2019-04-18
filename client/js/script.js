setInterval( update, 500 );

function update() 
{
    var id = document.getElementById("idnumber");
    fetch('/browserGet?id='+id.value)
    .then(res => res.json())
    .then(function (body) {
         console.log(body)
        
        switch(body["but"]) {
            case 1:
                square(body["pot"]);
                break;
            case 2:
                circle(body["pot"]);
                break;
            case 3:
                triangle(body["pot"]);
                break;
            // case 4:
            //     pacman(body["pot"]);
            //     break;    
            default:
                break;
        }
    });
}



function square(col) {
    var element = document.getElementById("shape");
    element.className = 'square';
    element.style.backgroundColor = '#'+col;
}

function circle(col) {
    var element = document.getElementById("shape");
    element.className = 'circle';
    element.style.backgroundColor = '#'+col;
}

function triangle(col) {
    var element = document.getElementById("shape");
    element.className = 'triangle';
    element.style.backgroundColor = 'white';
    element.style.borderBottomColor = '#'+col;
}

function pacman(col) {
    var element = document.getElementById("shape");
    element.className = 'pacman';
    element.style.backgroundColor = 'white';
    element.style.borderBottomColor = '#'+col;
    element.style.borderTopColor = '#'+col;
    element.style.borderLeftColor = '#'+col;
}
