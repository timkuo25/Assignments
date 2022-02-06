arr_pics = ["images/1.jpeg",
            "images/2.jpeg",
            "images/3.jpeg",
            "images/4.jpeg",
            "images/5.jpeg",
            "images/6.jpeg"]

const display = document.getElementById("display")
const next = document.getElementById("next")
const prev = document.getElementById("previous")

let cur = 0
display.setAttribute('src', arr_pics[cur])
prev.classList.add("disabled")
const change_pic = forth => {
    if (forth){
        if (cur === 5) return
        else{
            if (cur === 4) next.classList.add("disabled")
            if (cur === 0) prev.classList.remove("disabled")
            cur += 1
            display.setAttribute('src', arr_pics[cur])
            return
        } 
    }
    else{
        if (cur === 0) return
        else{
            if (cur === 1) prev.classList.add("disabled")
            if (cur === 5) next.classList.remove("disabled")
            cur -= 1
            display.setAttribute('src', arr_pics[cur])
            return
        } 
    }
}

next.addEventListener("click", () => change_pic(true))
prev.addEventListener("click", () => change_pic(false))