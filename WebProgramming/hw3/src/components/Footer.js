const Footer = ({ totalNum, doneNum, handleMode, handleClear }) => {
    if (totalNum === 0) return null;
    return (
        <footer className="todo-app__footer" id="todo-footer">
            <div className="todo-app__total">{totalNum - doneNum} left</div>
            <ul className="todo-app__view-buttons">
                <li><button onClick={(e) => handleMode(e.target.innerText)}>All</button></li>
                <li><button onClick={(e) => handleMode(e.target.innerText)}>Active</button></li>
                <li><button onClick={(e) => handleMode(e.target.innerText)}>Completed</button></li>
            </ul>
            <div className="todo-app__clean">
                {doneNum > 0 ? <button onClick={() => handleClear()}>Clear completed</button> : null}
            </div>
        </footer>
    );
}

export default Footer;
