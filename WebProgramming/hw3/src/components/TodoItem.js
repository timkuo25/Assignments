const TodoItem = ({ content, id, done, handleDone, handleDel }) => {
    const crossLine = {
        textDecoration: 'line-through',
        opacity: '0.5'
    };

    return (         
        <li className="todo-app__item">
            <div className="todo-app__checkbox" >
                <input type="checkbox" id={id} onClick={() => handleDone(id)}/>
                <label htmlFor={id} style={done ? {background: "#26ca299b"} : {}}></label>
            </div>
            <h1 className="todo-app__item-detail" style={done ? crossLine : {}}>{content}</h1>
            <img src="./img/x.png" className="todo-app__item-x" alt="" onClick={() => handleDel(id)}/>
        </li>
    );
}

export default TodoItem;