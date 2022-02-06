import React from "react";

const Header = ({ text }) => {
    return (
        <header className="todo-app__header">
            <h1 className="todo-app__title" >{text}</h1>
        </header>
    );
}

export default Header;
