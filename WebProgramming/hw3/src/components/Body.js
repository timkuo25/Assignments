import TodoItem from "./TodoItem";

const Body = ({ mode, todos, handleEnter, handleDone, handleDel }) => {
    let showTodos = todos;
    if (mode === 'Active') showTodos = showTodos.filter(item => !item.done);
    if (mode === 'Completed') showTodos = showTodos.filter(item => item.done);

    return (
        <section className="todo-app__main">
            <input className="todo-app__input" placeholder="What needs to be done" onKeyPress={(e) => {
                if (e.key === 'Enter') handleEnter(e.target.value, e);
            } }/>
            <ul className="todo-app__list" id="todo-list">
                {showTodos.map(item =>
                    <TodoItem 
                        content={item.content}
                        key={item.id}
                        id={item.id}
                        done={item.done}
                        handleDone={handleDone}
                        handleDel={handleDel}
                    />
                )}
            </ul>
        </section>
    );
}

export default Body;
